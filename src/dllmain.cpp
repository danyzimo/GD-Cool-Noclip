#include "includes.h"
#include "b1_layer.h"

bool nc = false;

auto test(CCSprite* toggleOn, CCSprite* toggleOff)
{
    return (nc) ? toggleOn : toggleOff;
}

bool (__thiscall* MenuLayer_init)(CCLayer* self);
bool __fastcall MenuLayer_init_H(CCLayer* self, void*) {

    if (!MenuLayer_init(self)) return false;
    auto menu = CCMenu::create();
    auto sprite_on = CCSprite::create("GJ_button_01.png");
    auto sprite_off = CCSprite::create("GJ_button_06.png");

    sprite_on->setPosition({ 100,150 });
    sprite_off->setPosition({ 120,150 });

    // CCSprite test & buttons
    auto btn1_on = gd::CCMenuItemSpriteExtra::create(
        sprite_on,
        self,
        menu_selector(b1_layer::create2)
    );
    auto btn1_off = gd::CCMenuItemSpriteExtra::create(
        sprite_off,
        self,
        menu_selector(b1_layer::create3)
    );

    // labels
    auto btn1_on_label = CCLabelBMFont::create("Noclip On", "goldFont.fnt");auto btn1_off_label = CCLabelBMFont::create("Noclip Off", "goldFont.fnt");
    btn1_off->setPosition({ 200,-100 });
    btn1_off->setRotation(45);
    btn1_off_label->setPosition({ 200,-60 });
    btn1_off_label->setScale(.5);
    btn1_on->setPosition({ 100,-100 });
    btn1_on->setRotation(45);
    btn1_on_label->setPosition({ 100,-60 });
    btn1_on_label->setScale(.5);


    menu->addChild(btn1_on_label);
    menu->addChild(btn1_on);
    menu->addChild(btn1_off);
    menu->addChild(btn1_off_label);
    menu->setPosition({200,200});

    self->addChild(menu);
}

DWORD WINAPI thread_func(void* hModule) {
    // initialize minhook
    MH_Initialize();

    /*
      `uintptr_t` is a type which is always the size of a pointer, in this case it is
      32 bit / 4 bytes. The usage of it here is kinda reduntant since this code is not
      cross-platform, however i think it looks neat and it makes it easy to tell if
      something is an address.
    */
    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    
    /*
      Here we are hooking MenuLayer::init, by giving the starting address of the function
      and what function to hook it to, as well as a trampoline, which is used to call the
      original function without going through our hook.

      Hooking is quite a complex process and can be confusing to beginners, so if you
      want a more detailed explanation you can watch this video by Guided Hacking
      https://www.youtube.com/watch?v=jTl3MFVKSUM (in the video he makes his own
      hook method, however here we just use MinHook as its a lot simpler to use
      and does the dissassembly part for us).
      To give a basic explanation: hooking a function means overwriting its first few
      instructions, making it redirect code execution to our own function.

      Hooking the init function of a node (in this case a layer) is a common practice as
      that is where you should initialize the elements in the node, and in a hook, you
      can add your own nodes.
    */
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1907b0),
        reinterpret_cast<void*>(&MenuLayer_init_H),
        reinterpret_cast<void**>(&MenuLayer_init) // note the &, this gets the address of the variable
    );

    // enable all hooks you've created with minhook
    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

/*
This function runs whenever we inject our dll, this is present
in pretty much every mod, and is just winapi boilerplate
*/
BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        auto h = CreateThread(0, 0, thread_func, handle, 0, 0);
        if (h)
            CloseHandle(h);
        else
            return FALSE;
    }
    return TRUE;
}