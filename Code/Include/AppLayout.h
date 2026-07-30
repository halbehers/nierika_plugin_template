#pragma once

#include <nierika_dsp/nierika_dsp.h>

#include "Component/SettingsWindow.h"

class AppLayout final : public nlayout::AppLayout,
                         public nelement::SVGButton::OnClickListener
{
public:
    explicit AppLayout(ndsp::ParameterManager& parameterManager);
    ~AppLayout() override;

    void resized() override;

    void onButtonClick(const std::string& componentID) override;

private:
    nelement::SVGButton _settings;

    nlayout::WindowsManager _windowsManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppLayout)
};
