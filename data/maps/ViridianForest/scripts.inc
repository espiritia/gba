ViridianForest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianForest_OnTransition
	.byte 0

ViridianForest_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_VIRIDIAN_FOREST
	end

ViridianForest_EventScript_Youngster::
	msgbox ViridianForest_Text_FriendsItchingToBattle, MSGBOX_NPC
	end

ViridianForest_EventScript_Boy::
	msgbox ViridianForest_Text_RanOutOfPokeBalls, MSGBOX_NPC
	end

ViridianForest_EventScript_TrainerTips1::
	msgbox ViridianForest_Text_AvoidGrassyAreasWhenWeak, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips2::
	msgbox ViridianForest_Text_UseAntidoteForPoison, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips3::
	msgbox ViridianForest_Text_ContactOakViaPCToRatePokedex, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips4::
	msgbox ViridianForest_Text_CantCatchOwnedMons, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips5::
	msgbox ViridianForest_Text_WeakenMonsBeforeCapture, MSGBOX_SIGN
	end

ViridianForest_EventScript_ExitSign::
	msgbox ViridianForest_Text_LeavingViridianForest, MSGBOX_SIGN
	end

ViridianForest_EventScript_Rick::
	trainerbattle_single TRAINER_BUG_CATCHER_RICK, ViridianForest_Text_RickIntro, ViridianForest_Text_RickDefeat
	msgbox ViridianForest_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Doug::
	trainerbattle_single TRAINER_BUG_CATCHER_DOUG, ViridianForest_Text_DougIntro, ViridianForest_Text_DougDefeat
	msgbox ViridianForest_Text_DougPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Sammy::
	trainerbattle_single TRAINER_BUG_CATCHER_SAMMY, ViridianForest_Text_SammyIntro, ViridianForest_Text_SammyDefeat
	msgbox ViridianForest_Text_SammyPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Anthony::
	trainerbattle_single TRAINER_BUG_CATCHER_ANTHONY, ViridianForest_Text_AnthonyIntro, ViridianForest_Text_AnthonyDefeat
	msgbox ViridianForest_Text_AnthonyPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Charlie::
	trainerbattle_single TRAINER_BUG_CATCHER_CHARLIE, ViridianForest_Text_CharlieIntro, ViridianForest_Text_CharlieDefeat
	msgbox ViridianForest_Text_CharliePostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_MewClearingTrigger::
	lockall
	goto_if_not_defeated TRAINER_CAMPER_LIAM, ViridianForest_EventScript_MewClearingSkip
	goto_if_unset FLAG_BROCK_GAVE_MOSS_QUEST, ViridianForest_EventScript_MewClearingSkip
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 1
	goto ViridianForest_EventScript_MewEncounter
	end

ViridianForest_EventScript_MewClearingSkip::
	releaseall
	end

ViridianForest_EventScript_MewEncounter::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	msgbox ViridianForest_Text_FoundMossThenSound, MSGBOX_DEFAULT
	waitse
	playse SE_M_CONFUSE_RAY
	waitse
	clearflag FLAG_HIDE_MEW_FOREST_ENCOUNTER
	playbgm MUS_MYSTERY_GIFT, 0
	applymovement LOCALID_VIRIDIAN_FOREST_MEW, ViridianForest_Movement_MewApproach
	waitmovement 0
	waitse
	playmoncry SPECIES_MEW, CRY_MODE_ENCOUNTER
	waitmoncry
	textcolor NPC_TEXT_COLOR_MON
	msgbox ViridianForest_Text_MewVoice1, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_MewVoice2, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_MewVoice3, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_MewVoice4, MSGBOX_DEFAULT
	applymovement LOCALID_VIRIDIAN_FOREST_MEW, Common_Movement_ExclamationMark
	waitmovement 0
	waitse
	playmoncry SPECIES_MEW, CRY_MODE_NORMAL
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_VIRIDIAN_FOREST_MEW
	fadescreen FADE_FROM_BLACK
	clearflag FLAG_HIDE_AGATHA_FOREST_AMBUSH
	clearflag FLAG_HIDE_FOREST_AMBUSH_GRUNT1
	clearflag FLAG_HIDE_FOREST_AMBUSH_GRUNT2
	playse SE_M_STRENGTH
	applymovement LOCALID_VIRIDIAN_FOREST_AGATHA, ViridianForest_Movement_AgathaApproach
	applymovement LOCALID_VIRIDIAN_FOREST_GRUNT1, ViridianForest_Movement_GruntApproachLeft
	applymovement LOCALID_VIRIDIAN_FOREST_GRUNT2, ViridianForest_Movement_GruntApproachRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox ViridianForest_Text_AgathaConfront1, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_PlayerLies, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_AgathaDemandPokedex, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_PlayerRefuses, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_AgathaOrdersArrest, MSGBOX_DEFAULT
	applymovement LOCALID_VIRIDIAN_FOREST_GRUNT1, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_VIRIDIAN_FOREST_GRUNT2, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 10
	setvar VAR_0x8007, 3
	special ShakeScreen
	waitse
	playse SE_M_EARTHQUAKE
	msgbox ViridianForest_Text_OnixEmerges, MSGBOX_DEFAULT
	fadescreen FADE_TO_WHITE
	waitse
	playse SE_M_SANDSTORM
	msgbox ViridianForest_Text_OnixSandstormRescue, MSGBOX_DEFAULT
	applymovement LOCALID_PLAYER, ViridianForest_Movement_SetInvisible
	waitmovement 0
	playse SE_M_DIG
	delay 60
	fadescreen FADE_FROM_WHITE
	msgbox ViridianForest_Text_AgathaGivesUp, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_LandInSecretCave, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_VIRIDIAN_FOREST_AGATHA
	removeobject LOCALID_VIRIDIAN_FOREST_GRUNT1
	removeobject LOCALID_VIRIDIAN_FOREST_GRUNT2
	@ TODO: sustituir por "warphole MAP_PEWTER_CITY_SECRET_CAVE" en cuanto exista esa cueva
	@ (tarea futura). Warp temporal para no dejar al jugador bloqueado con la pantalla en negro.
	warp MAP_ROUTE2_VIRIDIAN_FOREST_NORTH_ENTRANCE, 1
	waitstate
	fadescreen FADE_FROM_BLACK
	releaseall
	end

ViridianForest_Movement_MewApproach::
	walk_slower_down
	step_end

ViridianForest_Movement_AgathaApproach::
	walk_up
	walk_up
	step_end

ViridianForest_Movement_GruntApproachLeft::
	walk_up
	walk_right
	step_end

ViridianForest_Movement_GruntApproachRight::
	walk_up
	walk_left
	step_end

ViridianForest_Movement_SetInvisible::
	set_invisible
	step_end
