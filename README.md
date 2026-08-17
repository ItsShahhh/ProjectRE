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

## Player Mechanics

### Interactable IK

_Note:_ The first two seconds of the gif is interactable with IK off and the rest of the clip is with IK on
<img width="1152" height="594" alt="IKPickableItem" src="https://github.com/user-attachments/assets/0e37186c-54f9-4fa2-90c5-a0acd3f19043" />

**Overview:** Interact with pickable will activate IK that move hand to the location of the interact pickable item 
**Technical Implementation:**
- Line Trace will shoot that will check if the pickable object type is World Static and implement UInteractableInterface https://github.com/ItsShahhh/ProjectRE/blob/255f34115a3fa9d4f972917236253e76c6885680/Source/ProjectRE/InteractableTrace.cpp#L32-L56
- Player need to face the interact item to be able to pick it up https://github.com/ItsShahhh/ProjectRE/blob/255f34115a3fa9d4f972917236253e76c6885680/Source/ProjectRE/InteractableTrace.cpp#L58-L85


### Parkour (Mantle and Vaulting)


<img width="800" height="400" alt="NoIKVaulting" src="https://github.com/user-attachments/assets/3e2c753e-98e0-475a-87ec-28466ddac0d5" />  <img width="800" height="400" alt="IKVaulting" src="https://github.com/user-attachments/assets/fca2d753-67e3-46ce-a4c8-3a88ad043578" />


## User Widget (UI)

<img width="800" height="400" alt="ShowingAmmoUI" src="https://github.com/user-attachments/assets/4a33c092-82c6-4a60-8c96-744c3b766e40" />

**Overview:** Aiming Weapon will show max ammo and current ammo at the side of weapon
**Technical Implementation:**
- User Widget for showing ammo will be visible aiming an weapon https://github.com/ItsShahhh/ProjectRE/blob/e680f3df1ca6a13eba11d90f97bae90d8d63ea70/Source/ProjectRE/WeaponBase.cpp#L283
- Based on the current ammo it will updated on the Ammo User Widget using event dispatcher
  
[Broadcast for checking ammo when shooting weapon]
https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L110
[Broadcast for checking ammo when reload weapon]
https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L257


<img width="400" height="400" alt="InventoryItem" src="https://github.com/user-attachments/assets/e9f615c4-cf7c-45b9-b891-fd28ad843af6" />

**Overview:** Stackable item inventory system and can replace unoccupied item slot with new item on the same slot
**Technical Implementation:**
- Inventory is refresh/checks every time inventory is open https://github.com/ItsShahhh/ProjectRE/blob/93dbef5f9bd8794a8c38a90650c3149a3b8292e9/Source/ProjectRE/InventoryComponent.cpp#L149-L174
- Check will happen inside of an array for any potential item that meet the requirement (Which is Gameplay Tag and Empty Slot) to be added to the inventory array https://github.com/ItsShahhh/ProjectRE/blob/93dbef5f9bd8794a8c38a90650c3149a3b8292e9/Source/ProjectRE/InventoryComponent.cpp#L158-L166
- Event Dispatcher will be broadcast at the end of array to inventory user widget https://github.com/ItsShahhh/ProjectRE/blob/93dbef5f9bd8794a8c38a90650c3149a3b8292e9/Source/ProjectRE/InventoryComponent.cpp#L169
- Same type of ammo inside inventory is stackable


<img width="400" height="400" alt="HotbarWeaponSlots" src="https://github.com/user-attachments/assets/e7829fe2-3948-49eb-9a76-55a9c88bbe44" />

**Overview:** Only Weapon is added onto the hotbar and can be move from slots to slots
**Technical Implementation:**
- Picking up weapon will automatically added to hotbar array https://github.com/ItsShahhh/ProjectRE/blob/93dbef5f9bd8794a8c38a90650c3149a3b8292e9/Source/ProjectRE/InventoryComponent.cpp#L63
- Dragged on hotbar slot and it will check that slot for texture then attach it to mouse cursor (This function is inside blueprint)
- When Dropped onto another hotbar slot that already occupied with weapon (Which is Not Empty) it revert back to previous dragged slot (This function is inside blueprint)
