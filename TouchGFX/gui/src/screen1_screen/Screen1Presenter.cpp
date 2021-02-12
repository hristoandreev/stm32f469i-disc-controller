#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

void Screen1Presenter::wifiScanningDone(const char *res) {
    view.updateAccessPoints(res);
}

void Screen1Presenter::updateProgress(int value) {
    view.updateApScanningProgress(value);
}

void Screen1Presenter::hideProgress() {
    view.hideProgress();
}

void Screen1Presenter::clearAPsList() {
    view.wifiScrollListClean();
}
