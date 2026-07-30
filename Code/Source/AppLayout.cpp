#include "AppLayout.h"

AppLayout::AppLayout(ndsp::ParameterManager& parameterManager):
    nlayout::AppLayout(parameterManager),
    _settings("settings", nui::Icons::getGear()),
    _windowsManager(*this)
{
    _settings.setIconSize(24.f);
    _settings.addOnClickListener(this);
    _windowsManager.createWindow(std::make_unique<component::SettingsWindow>("settings", _windowsManager));

    getLayout().setGap(16.f);
    getLayout().setDisplayGrid(false);
    getLayout().setResizableLineConfiguration({ .displayLine = false });

#if JUCE_MAC
    getLayout().setMargin(24.f, 24.f + 16.f, 24.f, 24.f);
#else
    getLayout().setMargin(24.f, 0.f, 24.f, 24.f);
#endif

    getLayout().init({ 1, 1 }, { 1, 1 });

    getLayout().setFixedRowHeight(0, 60.f);
    getLayout().setFixedColumnWidth(0, 32.f);

    getLayout().addComponent(_settings, 0, 0, 1, 1);
}

AppLayout::~AppLayout()
{
    _settings.removeListener(this);
}

void AppLayout::resized()
{
    nlayout::AppLayout::resized();

    _windowsManager.setBounds(getLocalBounds());
}

void AppLayout::onButtonClick(const std::string& componentID)
{
    if (componentID == _settings.getComponentID())
    {
        _windowsManager.showWindow("settings");
    }
}
