#include "pch.h"
#include "application.h"
#if __has_include("application.g.cpp")
#include "application.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::Windows3DTouchDemo::implementation
{
    int32_t application::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void application::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void application::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
