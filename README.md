# ZeroG
 
 
ZeroG is a Purley Portfolio project to demonstrate my coding standards and abilities.

Code class description.
Here I will try to explain a few of the Major classes I use and why I designed them in this way.


USTRUCT()

FWeaponGroup

Variables:

FString Group

This is used to organize individual weapon groups and our ship,

imagine you have one group named Primary and one Called Secondary.


TArray<UBaseWeaponSlot*> WeaponSlots;

The WeaponSlots array holds a pointer to each weapon slot within its group to later make it easier to access and execute functions.

With this information, we now can start creating some functions to access this data and create some actual gameplay.

we now use several functions to execute code, which are.


void ShootGroup(bool Shooting)

When this function is called it will automatically loop over all weapons in the group and tell them to execute their shooting event.


void SetRotation(FWeaponInfo& Info)

This function loops over all weapons in the Group and Sets their rotation depending on what type of Gimbal the Slot they are attached to have, we currently have 3 different Gimbals, Static, Gimbal, and Auto.

by putting this function directly into the struct it makes it really easy to access data and execute specific functions. instead of having to write some logic in every class, we want to rotate and shoot our weapon the data itself handles this for us.