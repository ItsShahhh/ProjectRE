# ProjectRE (Project Resident Evil)

Inspired by Resident Evil Which an Horror/Shooter A Short Project Showcase Created By Me Focusing On Simple and Complex Mechanics Such As Inventory, Parkour, Weapon, Enemy AI, etc.
_Noted This Also My First Ever C++ Project and i learned a lot of new ways to code_

---

## Weapon Mechanics

<img width="800" height="400" alt="WeaponRecoil" src="https://github.com/user-attachments/assets/704be8a8-f37a-49c2-b7fa-846657fe020d" />

**Overview:** Line Trace for shooting weapon will be based on each weapon bullet pallet
**Technical Implementation:**
- Inside of weapon base it will calculation will be made using for each loop referencing each weapon bullet pallet float (https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L88-L152)
- Based on the numbers of the bullet pallet weapon will shoot that amount https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.h#L67-L68
- Each bullet pallet will also been given random spread radius that also be calculated inside for each loop https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L88-L152


**Overview:** Adding Weapon Recoil by calculating current camera recoil and adding new weapon recoil to AddControllerYawInput
**Technical Implementation:**
- Include Weapon Recoil Force float variable inside weapon base https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.h#L58-L59
- Inside event tick Current Weapon Recoil will get override by new Weapon Recoil Force and Last Weapon Recoil will saved the old Current Weapon Recoil    https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/ProjectRECharacter.cpp#L242-L256
- On event tick also calculated between all those floats and added it to AddControllerYawInput https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/ProjectRECharacter.cpp#L242-L256


## User Widget (UI)

<img width="800" height="400" alt="ShowingAmmoUI" src="https://github.com/user-attachments/assets/4a33c092-82c6-4a60-8c96-744c3b766e40" />

**Overview:** Aiming Weapon will show max ammo and current ammo at the side of weapon
**Technical Implementation:**
- User Widget for showing ammo will be visible aiming an weapon https://github.com/ItsShahhh/ProjectRE/blob/e680f3df1ca6a13eba11d90f97bae90d8d63ea70/Source/ProjectRE/WeaponBase.cpp#L283
- Based on the current ammo it will updated on the Ammo User Widget using event dispatcher
  
[This broadcast is for when shooting weapon]
https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L110
[This broadcast is for when reloading weapon]
https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L257
