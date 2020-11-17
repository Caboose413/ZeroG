# ZeroG
 
 
ZeroG is a Portfolio project to demonstrate my coding standards and abilities.

Code class description.

Here I will try to explain a few of the Major classes I use and why I designed them in this way.


USTRUCT()

FWeaponGroup

Variables:

FString Group

This is used to organize individual weapon groups on our ship,

imagine you have one group named Primary and one Called Secondary.


TArray<UBaseWeaponSlot*> WeaponSlots;

The WeaponSlots array holds a pointer to each weapon slot within its group to later make it easier to access and execute functions.

With this information, we now can start creating some functions to access this data and create some actual gameplay.

we now use several functions to execute code, which are.


void ShootGroup(bool Shooting)

When this function is called it will automatically loop over all weapons in the group and tell them to execute their shooting event.


void SetRotation(FWeaponInfo& Info)

This function loops over all weapons in the Group and Sets their rotation depending on what type of Gimbal the Slot they are attached to have,
we currently have 3 different Gimbals, Static, Gimbal, and Auto.

by putting these functions directly into the Struct it makes it really easy to access data and execute specific functions.
instead of having to write logic in every class we want to use our weapons in to for example rotate and shoot,
all we have todo now is to acces our Struct and execute our functions within it and the Struct itself will handle the rest for us.