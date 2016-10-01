/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
//
// Created by simon on 2015-12-20.
//

#include <ControlsManager.h>
#include <map>
#include <vector>
#include <ControlStatus.h>
#include <string>


ControlsManager* ControlsManager::getInstance() {

    static ControlsManager instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return &instance;

}

ControlsManager::ControlsManager() { }
ControlsManager::~ControlsManager() {
    for(auto i = functions.begin(); i!= functions.end() ; i++)
        for(auto j = i->second.begin() ; j != i->second.end() ; j++)
            delete (*j); //Delete the buttons
}

void ControlsManager::clearBindings() {
    functions = std::map<int,std::vector<Input*>>();
}

void ControlsManager::addBinding(int function, Input* button) {
    addBindings(function,{button});
}

void ControlsManager::addBindings(int function, std::initializer_list<Input*> buttons) {
    functionCollection::iterator elem = functions.find(function);
    if(elem == functions.end()){
        addBindingsForNewFunction(function,buttons);
    }else {
        addBindingsForExistingFunction(function,buttons,elem->second);
    }
}

void ControlsManager::addBindingsForNewFunction(int function, std::initializer_list<Input *> buttons) {
    std::vector<Input *> list;
    bool firstDuality = (*buttons.begin())->isDual();
    for (auto it = buttons.begin(); it != buttons.end(); it++) {
        if((*it)->isDual() != firstDuality)
            throw "The duality of the bindings were different for function " + std::to_string(function) + ".";
        list.insert(list.end(), *it);
    }
    functions.insert(std::pair<int,std::vector<Input*>>(function,list));
}

void ControlsManager::addBindingsForExistingFunction(int function, std::initializer_list<Input *> buttons,
                                                     std::vector<Input *> bindings) {
    bool shouldBeDual = bindings.front()->isDual();
    for (auto it = buttons.begin(); it != buttons.end(); it++) {
        if((*it)->isDual() != shouldBeDual)
            throw ("The function " + std::to_string(function) + " is " + (shouldBeDual ? "" : "not ") +
                   "dual but a binding was " + (!shouldBeDual ? "not " : "") + "dual.");
        removeBindingsWithActivator(&bindings, (*it)->getActivator());
        bindings.insert(bindings.end(), *it);
    }
    functions.insert(std::pair<int,std::vector<Input*>>(function,bindings));
}

void ControlsManager::removeBindingsWithActivator(std::vector<Input *> *bindings,
                                                  ControlStatus::Activator toRemove) {
    for (auto it = bindings->begin(); it != bindings->end();) {
        if ((*it)->getActivator() == toRemove) {
            Input* remove = *it;
            it = bindings->erase(it);
            delete remove;
        }else
            it++;
    }
}

ControlStatus ControlsManager::getStatus(int function) {
    std::vector<Input*> buttons;
    functionCollection::iterator elem = functions.find(function);
    if(elem == functions.end())
        return  ControlStatus();
    buttons = elem->second;
    ControlStatus status = ControlStatus();
    for(auto it = buttons.begin(); it != buttons.end() ; it++)
        status.merge((*it)->getStatus());
    return status;
}


