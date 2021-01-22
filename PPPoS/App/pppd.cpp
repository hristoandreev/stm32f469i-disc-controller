//
// Created by hristo on 1/3/21.
//

/* Includes ------------------------------------------------------------------*/
#include "pppd.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include <netif/ppp/pppos.h>
#include <lwip/dns.h>
#include <netif/ppp/pppapi.h>
#include <queue.h>
#include <logg.h>
#include "netif/ppp/ppp.h"
#include "uart.h"

/* The PPP control block */
ppp_pcb *ppp;

/* The PPP IP interface */
struct netif ppp_netif1;

#define DATA_SIZE 2048U
static u8_t ppp_data[DATA_SIZE];

extern UART_HandleTypeDef huart6;

struct uart pppos_uart = {
        huart: &huart6,
        iq_size_bytes: 1024,
        oq_size_bytes: 1024
};

void modem_reset() {
    LOG_W(LOG_DBG_ON, "modem_reset", "Resetting...");
    HAL_GPIO_WritePin(ESP32_RESET_GPIO_Port, ESP32_RESET_Pin, GPIO_PIN_RESET);
    (void)osDelay(100 / portTICK_RATE_MS);
    HAL_GPIO_WritePin(ESP32_RESET_GPIO_Port, ESP32_RESET_Pin, GPIO_PIN_SET);
    LOG_W(LOG_DBG_ON, "modem_reset", "Done.");
}

__attribute__((noreturn))
void PPPoS_Task(void *argument) {
    (void) argument;

    dbgInit();

    LOG_D(LOG_DBG_ON, "PPPoS_Task", "Task started.");

    modem_reset();

    uartInit(&pppos_uart);

    /* Initialize the LwIP stack with RTOS */
    tcpip_init(nullptr, nullptr);

    while(true) {
        /*
        * Create a new PPPoS interface
        */
        LOG_I(LOG_DBG_ON, "PPPoS_Task", "Creating new PPPoS interface...");
        ppp = pppapi_pppos_create(
                &ppp_netif1,
               [](ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx) {
                    if(len > 0) {
//                        LOG_I(LOG_DBG_ON, "PPPoS_Task", "PPPoS transmit %lu byte(s)", len);
                        return static_cast<u32_t>(uartWrite(&pppos_uart, data, len, 50));
                    }
                    return static_cast<u32_t>(0);
               },
               [](ppp_pcb *pcb, int err_code, void *ctx) {
                   struct netif *pppif = ppp_netif(pcb);
                   LWIP_UNUSED_ARG(ctx);

                   switch (err_code) {
                       case PPPERR_NONE:
                           LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Connected.");
#if PPP_IPV4_SUPPORT
                           LOG_I(LOG_DBG_ON, "PPPoS_Task", "   our_ipaddr  = %s", ipaddr_ntoa(&pppif->ip_addr));
                           LOG_I(LOG_DBG_ON, "PPPoS_Task", "   his_ipaddr  = %s", ipaddr_ntoa(&pppif->gw));
                           LOG_I(LOG_DBG_ON, "PPPoS_Task", "   netmask     = %s", ipaddr_ntoa(&pppif->netmask));
#if LWIP_DNS
                           const ip_addr_t *ns;
                           ns = dns_getserver(0);
                           LOG_I(LOG_DBG_ON, "PPPoS_Task", "   dns1        = %s", ipaddr_ntoa(ns));
                           ns = dns_getserver(1);
                           LOG_I(LOG_DBG_ON, "PPPoS_Task", "   dns2        = %s", ipaddr_ntoa(ns));
#endif /* LWIP_DNS */
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
                           LOG_I(LOG_DBG_ON, "PPPoS_Task", "   our6_ipaddr = %s\n", ip6addr_ntoa(netif_ip6_addr(pppif, 0)));
#endif /* PPP_IPV6_SUPPORT */
                           break;
                       case PPPERR_PARAM: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Invalid parameter.");
                           break;
                       case PPPERR_OPEN: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Unable to open PPP session.");
                           break;
                       case PPPERR_DEVICE: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Invalid I/O device for PPP.");
                           break;
                       case PPPERR_ALLOC: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Unable to allocate resources.");
                           break;
                       case PPPERR_USER: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: User interrupt.");
                           break;
                       case PPPERR_CONNECT: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Connection lost.");
                           break;
                       case PPPERR_AUTHFAIL: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Failed authentication challenge.");
                           break;
                       case PPPERR_PROTOCOL: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Failed to meet protocol.");
                           break;
                       case PPPERR_PEERDEAD: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Connection timeout.");
                           break;
                       case PPPERR_IDLETIMEOUT: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Idle Timeout.");
                           break;
                       case PPPERR_CONNECTTIME: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Max connect time reached.");
                           break;
                       case PPPERR_LOOPBACK: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Loopback detected.");
                           break;
                       default: LOG_I(LOG_DBG_ON, "PPPoS_Task", "status_cb: Unknown error code %d.", err_code);
                           break;
                   }
                   /*
                    * This should be in the switch case, this is put outside of the switch
                    * case for example readability.
                    */

                   if (err_code == PPPERR_NONE) {
                       return;
                   }

                   /* ppp_close() was previously called, don't reconnect */
                   if (err_code == PPPERR_USER) {
                       /* ppp_free(); -- can be called here */
                       (void)ppp_free(ppp);
                       return;
                   }
               },nullptr);

        DBG_ASSERT(ppp != nullptr, "Failed!!!");

        LOG_I(LOG_DBG_ON, "PPPoS_Task", "Done.");

        /* Set this interface as default route */
        (void)pppapi_set_default(ppp);
        /* Ask the peer for up to 2 DNS server addresses. */
        ppp_set_usepeerdns(ppp, 1);
        /* Auth configuration, this is pretty self-explanatory */
        ppp_set_auth(ppp, PPPAUTHTYPE_NONE, "login", "password");

        LOG_I(LOG_DBG_ON, "PPPoS_Task", "PPPos connecting...");
        (void) pppapi_connect(ppp, 0);

        netif_set_default(&ppp_netif1);
        netif_set_link_up(&ppp_netif1);

        uartStartReceiving(&pppos_uart);
        uint32_t length;
        while (true) {
            length = uartRead(&pppos_uart, ppp_data, DATA_SIZE, 50);
            if (length > 0) {
                (void) pppos_input_tcpip(ppp, ppp_data, length);
//                LOG_I(LOG_DBG_ON, "PPPoS_Task", "PPPoS received %lu byte(s)", length);
            }

            if (ppp->phase == PPP_PHASE_DEAD) {
                /* This happens when the client disconnects */
                (void) pppapi_free(ppp);
                ppp = nullptr;
                modem_reset();
                break;
            }
        }
    }
}
