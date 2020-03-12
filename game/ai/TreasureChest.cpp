
#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "AI.h"

class rvTreasureChest : public idAI {
public:

	CLASS_PROTOTYPE(rvTreasureChest);

	rvTreasureChest(void);

	void				Spawn(void);
	void				Save(idSaveGame *savefile) const;
	void				Restore(idRestoreGame *savefile);

protected:

	virtual void		OnDeath(void);

private:

	CLASS_STATES_PROTOTYPE(rvTreasureChest);
};

CLASS_DECLARATION(idAI, rvTreasureChest)
END_CLASS

/*
================
rvTreasureChest::rvTreasureChest
================
*/
rvTreasureChest::rvTreasureChest() {
	SetSkin(declManager->FindSkin(spawnArgs.GetString("moveable_crate1_small")));
}

/*
================
rvTreasureChest::Spawn
================
*/
void rvTreasureChest::Spawn(void) {
	LoadAF(NULL, true);
}

/*
================
rvTreasureChest::Save
================
*/
void rvTreasureChest::Save(idSaveGame *savefile) const {
}

/*
================
rvTreasureChest::Restore
================
*/
void rvTreasureChest::Restore(idRestoreGame *savefile) {
}



/*
================
rvTreasureChest::OnDeath
================
*/
void rvTreasureChest::OnDeath(void) {

	PlayEffect("fx_bloodyburst", animator.GetJointHandle("chest"));

	const char *value;
	idEntity* dropitem = NULL;
	idDict	  args;


	int itemDropped = rand() % 14;
	switch (itemDropped){
	case 0:
		value = "weapon_blaster";
		break;
	case 1:
		value = "weapon_dmg";
		break;
	case 2:
		value = "weapon_grenadelauncher";
		break;
	case 3:
		value = "weapon_hyperblaster";
		break;
	case 4:
		value = "weapon_lightninggun";
		break;
	case 5:
		value = "weapon_nailgun";
		break;
	case 6:
		value = "weapon_napalmgun";
		break;
	case 7:
		value = "weapon_railgun";
		break;
	case 8:
		value = "weapon_rocketlauncher";
		break;
	case 9:
		value = "weapon_shotgun";
		break;
	case 10:
		value = "powerup_quad_damage";
		break;
	case 11:
		value = "powerup_haste";
		break;
	case 12:
		value = "powerup_regeneration";
		break;
	case 13:
		value = "powerup_invisibility";
		break;
	case 14:
		value = "powerup_team_damage_mod";
		break;
	}


	args.Set("classname", value);
	args.SetVector("origin", GetPhysics()->GetOrigin());
	args.SetInt("angle", move.current_yaw);
	gameLocal.SpawnEntityDef(args, &dropitem);

	return idAI::OnDeath();
}


/*
===============================================================================

States

===============================================================================
*/

CLASS_STATES_DECLARATION(rvTreasureChest)
END_CLASS_STATES
