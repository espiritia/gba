ViridianForest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianForest_OnTransition
	.byte 0

ViridianForest_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_VIRIDIAN_FOREST
	setflag FLAG_HIDE_MEW_FOREST_ENCOUNTER
	setflag FLAG_HIDE_AGATHA_FOREST_AMBUSH
	setflag FLAG_HIDE_FOREST_AMBUSH_GRUNT1
	setflag FLAG_HIDE_FOREST_AMBUSH_GRUNT2
	setflag FLAG_HIDE_FOREST_ONIX_DEBRIS
	@ Si se sale o recarga tras la oscuridad, se repite esa etapa para restaurarla
	call_if_eq VAR_MAP_SCENE_VIRIDIAN_FOREST, 3, ViridianForest_EventScript_ResetMewDarkness
	end

ViridianForest_EventScript_ResetMewDarkness::
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 2
	return

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

@ Etapa 1: sonido cristalino
ViridianForest_EventScript_MewSoundTrigger::
	lockall
	goto_if_not_defeated TRAINER_CAMPER_LIAM, ViridianForest_EventScript_MewClearingSkip
	goto_if_unset FLAG_BROCK_GAVE_MOSS_QUEST, ViridianForest_EventScript_MewClearingSkip
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 1
	playse SE_M_HEAL_BELL
	waitse
	msgbox ViridianForest_Text_CrystalSound, MSGBOX_DEFAULT
	releaseall
	end

ViridianForest_EventScript_MewClearingSkip::
	releaseall
	end

@ Etapa 2: Mew cruza a toda velocidad
ViridianForest_EventScript_MewDashTrigger::
	lockall
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 2
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_MEW, 18, 32
	addobject LOCALID_VIRIDIAN_FOREST_MEW
	playse SE_M_HEAL_BELL
	applymovement LOCALID_VIRIDIAN_FOREST_MEW, ViridianForest_Movement_MewDash
	waitmovement 0
	removeobject LOCALID_VIRIDIAN_FOREST_MEW
	releaseall
	end

@ Etapa 3: el ambiente se oscurece y aparece Mew
ViridianForest_EventScript_MewDarknessTrigger::
	lockall
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 3
	playbgm MUS_MYSTERY_GIFT, 0
	setvar VAR_0x8004, 9
	setvar VAR_0x8005, 6
	special FadeWeatherGammaTo
	delay 60
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_MEW, 22, 33
	addobject LOCALID_VIRIDIAN_FOREST_MEW
	turnobject LOCALID_VIRIDIAN_FOREST_MEW, DIR_WEST
	releaseall
	end

@ Etapa 4: al acercarse a Mew. Cada fila coloca al jugador en (20,33)
ViridianForest_EventScript_MewEncounterRow31::
	setvar VAR_0x8008, 31
	goto ViridianForest_EventScript_MewEncounter
	end

ViridianForest_EventScript_MewEncounterRow32::
	setvar VAR_0x8008, 32
	goto ViridianForest_EventScript_MewEncounter
	end

ViridianForest_EventScript_MewEncounterRow33::
	setvar VAR_0x8008, 33
	goto ViridianForest_EventScript_MewEncounter
	end

ViridianForest_EventScript_MewEncounterRow34::
	setvar VAR_0x8008, 34
	goto ViridianForest_EventScript_MewEncounter
	end

ViridianForest_EventScript_MewEncounterRow35::
	setvar VAR_0x8008, 35
	goto ViridianForest_EventScript_MewEncounter
	end

ViridianForest_EventScript_MewEncounter::
	lockall
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 4
	call ViridianForest_EventScript_Darken
	call_if_eq VAR_0x8008, 31, ViridianForest_EventScript_AlignFromRow31
	call_if_eq VAR_0x8008, 32, ViridianForest_EventScript_AlignFromRow32
	call_if_eq VAR_0x8008, 34, ViridianForest_EventScript_AlignFromRow34
	call_if_eq VAR_0x8008, 35, ViridianForest_EventScript_AlignFromRow35
	turnobject LOCALID_PLAYER, DIR_EAST
	waitse
	playmoncry SPECIES_MEW, CRY_MODE_ENCOUNTER
	waitmoncry
	textcolor NPC_TEXT_COLOR_MON
	msgbox ViridianForest_Text_MewVoice, MSGBOX_DEFAULT
	closemessage
	playse SE_M_TELEPORT
	removeobject LOCALID_VIRIDIAN_FOREST_MEW
	waitse
	delay 60
	@ Agatha y sus soldados llegan por el claro, desde fuera de la pantalla (oeste)
	addobject LOCALID_VIRIDIAN_FOREST_AGATHA
	addobject LOCALID_VIRIDIAN_FOREST_GRUNT1
	addobject LOCALID_VIRIDIAN_FOREST_GRUNT2
	applymovement LOCALID_VIRIDIAN_FOREST_AGATHA, ViridianForest_Movement_AgathaApproach
	applymovement LOCALID_VIRIDIAN_FOREST_GRUNT1, ViridianForest_Movement_Grunt1Approach
	applymovement LOCALID_VIRIDIAN_FOREST_GRUNT2, ViridianForest_Movement_Grunt2Approach
	delay 48
	turnobject LOCALID_PLAYER, DIR_WEST
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox ViridianForest_Text_AgathaConfront, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianForest_EventScript_AnswerNo
	msgbox ViridianForest_Text_AgathaTruthful, MSGBOX_DEFAULT
	goto ViridianForest_EventScript_SoldierDemandPokedex
	end

ViridianForest_EventScript_AnswerNo::
	msgbox ViridianForest_Text_AgathaCallsLiar, MSGBOX_DEFAULT
	goto ViridianForest_EventScript_SoldierDemandPokedex
	end

ViridianForest_EventScript_SoldierDemandPokedex::
	closemessage
	applymovement LOCALID_VIRIDIAN_FOREST_GRUNT1, ViridianForest_Movement_GruntConfiscate
	waitmovement 0
	turnobject LOCALID_PLAYER, DIR_NORTH
	textcolor NPC_TEXT_COLOR_MALE
	msgbox ViridianForest_Text_SoldierDemandPokedex, MSGBOX_DEFAULT
	goto ViridianForest_EventScript_OnixAttack
	end

ViridianForest_EventScript_OnixAttack::
	closemessage
	call ViridianForest_EventScript_LugiaShake
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox ViridianForest_Text_AgathaOnixHere, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_WHITE
	applymovement LOCALID_PLAYER, ViridianForest_Movement_SetInvisible
	waitmovement 0
	addobject LOCALID_VIRIDIAN_FOREST_HOLE1
	addobject LOCALID_VIRIDIAN_FOREST_HOLE2
	addobject LOCALID_VIRIDIAN_FOREST_HOLE3
	addobject LOCALID_VIRIDIAN_FOREST_ROCK1
	addobject LOCALID_VIRIDIAN_FOREST_ROCK2
	addobject LOCALID_VIRIDIAN_FOREST_ROCK3
	delay 120
	fadescreen FADE_FROM_WHITE
	call ViridianForest_EventScript_Darken
	call ViridianForest_EventScript_LugiaShake
	delay 30
	applymovement LOCALID_VIRIDIAN_FOREST_GRUNT2, ViridianForest_Movement_GruntReportToAgatha
	waitmovement 0
	turnobject LOCALID_VIRIDIAN_FOREST_AGATHA, DIR_WEST
	textcolor NPC_TEXT_COLOR_MALE
	msgbox ViridianForest_Text_SoldierChase, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox ViridianForest_Text_AgathaRetreat1, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_AgathaRetreat2, MSGBOX_DEFAULT
	msgbox ViridianForest_Text_AgathaRetreat3, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_VIRIDIAN_FOREST_AGATHA
	removeobject LOCALID_VIRIDIAN_FOREST_GRUNT1
	removeobject LOCALID_VIRIDIAN_FOREST_GRUNT2
	removeobject LOCALID_VIRIDIAN_FOREST_HOLE1
	removeobject LOCALID_VIRIDIAN_FOREST_HOLE2
	removeobject LOCALID_VIRIDIAN_FOREST_HOLE3
	removeobject LOCALID_VIRIDIAN_FOREST_ROCK1
	removeobject LOCALID_VIRIDIAN_FOREST_ROCK2
	removeobject LOCALID_VIRIDIAN_FOREST_ROCK3
	@ TODO: sustituir por "warphole MAP_PEWTER_CITY_SECRET_CAVE" en cuanto exista esa cueva
	@ (tarea futura). Warp temporal para no dejar al jugador bloqueado con la pantalla en negro.
	warp MAP_ROUTE2_VIRIDIAN_FOREST_NORTH_ENTRANCE, 1
	waitstate
	fadescreen FADE_FROM_BLACK
	releaseall
	end

@ Oscurece el ambiente (gamma del clima SHADE). Hay que reaplicarlo tras cada fundido.
ViridianForest_EventScript_Darken::
	delay 2
	setvar VAR_0x8004, 9
	setvar VAR_0x8005, 1
	special FadeWeatherGammaTo
	delay 10
	return

@ Temblor igual al de LUGIA en Navel Rock
ViridianForest_EventScript_LugiaShake::
	waitse
	playse SE_THUNDERSTORM_STOP
	setvar VAR_0x8004, 0  @ Vertical pan
	setvar VAR_0x8005, 3  @ Horizontal pan
	setvar VAR_0x8006, 4  @ Num shakes
	setvar VAR_0x8007, 2  @ Shake delay
	special ShakeScreen
	delay 30
	playse SE_THUNDERSTORM_STOP
	setvar VAR_0x8004, 0  @ Vertical pan
	setvar VAR_0x8005, 3  @ Horizontal pan
	setvar VAR_0x8006, 4  @ Num shakes
	setvar VAR_0x8007, 2  @ Shake delay
	special ShakeScreen
	delay 30
	return

ViridianForest_EventScript_AlignFromRow31::
	applymovement LOCALID_PLAYER, ViridianForest_Movement_PlayerDown2
	waitmovement 0
	return

ViridianForest_EventScript_AlignFromRow32::
	applymovement LOCALID_PLAYER, ViridianForest_Movement_PlayerDown1
	waitmovement 0
	return

ViridianForest_EventScript_AlignFromRow34::
	applymovement LOCALID_PLAYER, ViridianForest_Movement_PlayerUp1
	waitmovement 0
	return

ViridianForest_EventScript_AlignFromRow35::
	applymovement LOCALID_PLAYER, ViridianForest_Movement_PlayerUp2
	waitmovement 0
	return

ViridianForest_Movement_MewDash::
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	step_end

ViridianForest_Movement_PlayerDown2::
	walk_down
ViridianForest_Movement_PlayerDown1::
	walk_down
	step_end

ViridianForest_Movement_PlayerUp2::
	walk_up
ViridianForest_Movement_PlayerUp1::
	walk_up
	step_end

ViridianForest_Movement_AgathaApproach::
	walk_right
ViridianForest_Movement_Grunt2Approach::
	walk_right
ViridianForest_Movement_Grunt1Approach::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

ViridianForest_Movement_GruntConfiscate::
	walk_right
	walk_right
	walk_right
	walk_right
	face_down
	step_end

ViridianForest_Movement_GruntReportToAgatha::
	walk_right
	walk_right
	face_right
	step_end

ViridianForest_Movement_SetInvisible::
	set_invisible
	step_end
