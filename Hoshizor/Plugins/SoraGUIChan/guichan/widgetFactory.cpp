//
//  widgetFactory.cpp
//  Guichan_Sora
//
//  Created by Robert Bu(darkfall) 8/26/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "widgetFactory.hpp"

#include "guichan/widgets/button.hpp"
#include "guichan/widgets/checkbox.hpp"
#include "guichan/widgets/container.hpp"
#include "guichan/widgets/dropdown.hpp"
#include "guichan/widgets/icon.hpp"
#include "guichan/widgets/imagebutton.hpp"
#include "guichan/widgets/label.hpp"
#include "guichan/widgets/listbox.hpp"
#include "guichan/widgets/scrollarea.hpp"
#include "guichan/widgets/slider.hpp"
#include "guichan/widgets/radiobutton.hpp"
#include "guichan/widgets/tab.hpp"
#include "guichan/widgets/tabbedarea.hpp"
#include "guichan/widgets/textbox.hpp"
#include "guichan/widgets/textfield.hpp"
#include "guichan/widgets/window.hpp"

namespace gcn {
    
    void RegisterCoreWidgets() {
        static bool registerd = false;
        if(!registerd) {
            WidgetFactory& factory = WidgetFactory::Instance();
            
            factory.reg<Button>("button");
            factory.reg<CheckBox>("checkbox");
            factory.reg<Container>("container");
            factory.reg<DropDown>("dropdown");
            factory.reg<Icon>("icon");
            factory.reg<ImageButton>("imagebutton");
            factory.reg<Label>("label");
            factory.reg<ListBox>("listbox");
            factory.reg<ScrollArea>("scrollarea");
            factory.reg<Slider>("slider");
            factory.reg<RadioButton>("radiobutton");
            factory.reg<Tab>("tab");
            factory.reg<TabbedArea>("tabbedarea");
            factory.reg<TextBox>("textbox");
            factory.reg<TextField>("textfield");
            factory.reg<Window>("window");

            registerd = true;
        }
    }
    
} // namespace gcn
