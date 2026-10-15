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
	setflag FLAG_HIDE_FOREST_MOSS_SPARKLE
	@ Señuelos: solo durante el rastro; sus flags se conservan para no reaparecer si ya se gastaron
	call_if_ne VAR_MAP_SCENE_VIRIDIAN_FOREST, 2, ViridianForest_EventScript_HideDecoys
	@ Rastro del musgo en curso: los brillos vuelven a la posición del paso actual
	call_if_eq VAR_MAP_SCENE_VIRIDIAN_FOREST, 2, ViridianForest_EventScript_ShowSparkle
	@ Si se sale o recarga tras la oscuridad, se repite esa etapa para restaurarla
	call_if_eq VAR_MAP_SCENE_VIRIDIAN_FOREST, 4, ViridianForest_EventScript_ResetMewDarkness
	end

ViridianForest_EventScript_ShowSparkle::
	call ViridianForest_EventScript_PlaceSparkle
	return

ViridianForest_EventScript_HideDecoys::
	setflag FLAG_HIDE_FOREST_MOSS_DECOY1
	setflag FLAG_HIDE_FOREST_MOSS_DECOY2
	return

ViridianForest_EventScript_ResetMewDarkness::
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 3
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
	@ Empieza el rastro del musgo brillante
	setvar VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 0
	call ViridianForest_EventScript_PlaceSparkle
	addobject LOCALID_VIRIDIAN_FOREST_SPARKLE
	releaseall
	end

@ Etapa 2b: rastro del musgo brillante. 5 brillos; Cubone (en el equipo) indica hacia dónde sigue.
@ En los pasos 2-4 hay además 2 señuelos: Cubone da una pista para deducir cuál es el real.
ViridianForest_EventScript_SparkleTrigger::
ViridianForest_EventScript_SparkleInteract::
	lockall
	goto_if_questlog EventScript_ReleaseEnd
	goto_if_ne VAR_MAP_SCENE_VIRIDIAN_FOREST, 2, ViridianForest_EventScript_SparkleSkip
	setvar VAR_0x8004, SPECIES_CUBONE
	specialvar VAR_RESULT, IsSpeciesInPlayerParty
	goto_if_eq VAR_RESULT, FALSE, ViridianForest_EventScript_SparkleNoCubone
	special QuestLog_CutRecording
	playse SE_M_HEAL_BELL
	waitse
	playmoncry SPECIES_CUBONE, CRY_MODE_NORMAL
	waitmoncry
	textcolor NPC_TEXT_COLOR_MON
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 0, ViridianForest_EventScript_CuboneSniff1
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 1, ViridianForest_EventScript_CuboneSniff2
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 2, ViridianForest_EventScript_CuboneSniff3
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 3, ViridianForest_EventScript_CuboneSniff4
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 4, ViridianForest_EventScript_CuboneSniff5
	removeobject LOCALID_VIRIDIAN_FOREST_SPARKLE
	removeobject LOCALID_VIRIDIAN_FOREST_DECOY1
	removeobject LOCALID_VIRIDIAN_FOREST_DECOY2
	addvar VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 1
	goto_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 5, ViridianForest_EventScript_TrailComplete
	call ViridianForest_EventScript_CuboneClue
	closemessage
	call ViridianForest_EventScript_PlaceSparkle
	addobject LOCALID_VIRIDIAN_FOREST_SPARKLE
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 4, ViridianForest_EventScript_HideDecoys
	goto_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 4, ViridianForest_EventScript_SparkleSkip
	clearflag FLAG_HIDE_FOREST_MOSS_DECOY1
	clearflag FLAG_HIDE_FOREST_MOSS_DECOY2
	addobject LOCALID_VIRIDIAN_FOREST_DECOY1
	addobject LOCALID_VIRIDIAN_FOREST_DECOY2
	releaseall
	end

ViridianForest_EventScript_TrailComplete::
	closemessage
	call ViridianForest_EventScript_HideDecoys
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 3
	releaseall
	end

@ Señuelos: el musgo falso era un PARAS escondido. El señuelo se gasta y Cubone repite la pista
ViridianForest_EventScript_DecoyTrigger1::
ViridianForest_EventScript_DecoyInteract1::
	lockall
	goto_if_questlog EventScript_ReleaseEnd
	goto_if_set FLAG_HIDE_FOREST_MOSS_DECOY1, ViridianForest_EventScript_SparkleSkip
	setvar VAR_0x8008, 1
	goto ViridianForest_EventScript_Decoy
	end

ViridianForest_EventScript_DecoyTrigger2::
ViridianForest_EventScript_DecoyInteract2::
	lockall
	goto_if_questlog EventScript_ReleaseEnd
	goto_if_set FLAG_HIDE_FOREST_MOSS_DECOY2, ViridianForest_EventScript_SparkleSkip
	setvar VAR_0x8008, 2
	goto ViridianForest_EventScript_Decoy
	end

ViridianForest_EventScript_Decoy::
	goto_if_ne VAR_MAP_SCENE_VIRIDIAN_FOREST, 2, ViridianForest_EventScript_SparkleSkip
	setvar VAR_0x8004, SPECIES_CUBONE
	specialvar VAR_RESULT, IsSpeciesInPlayerParty
	goto_if_eq VAR_RESULT, FALSE, ViridianForest_EventScript_SparkleNoCubone
	special QuestLog_CutRecording
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox ViridianForest_Text_FakeMoss, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_0x8008, 1, ViridianForest_EventScript_RemoveDecoy1
	call_if_eq VAR_0x8008, 2, ViridianForest_EventScript_RemoveDecoy2
	setwildbattle SPECIES_PARAS, 6
	dowildbattle
	textcolor NPC_TEXT_COLOR_MON
	msgbox ViridianForest_Text_CuboneRemember, MSGBOX_DEFAULT
	call ViridianForest_EventScript_CuboneClue
	closemessage
	releaseall
	end

ViridianForest_EventScript_RemoveDecoy1::
	removeobject LOCALID_VIRIDIAN_FOREST_DECOY1
	return

ViridianForest_EventScript_RemoveDecoy2::
	removeobject LOCALID_VIRIDIAN_FOREST_DECOY2
	return

@ Pista lógica para identificar el musgo real del paso actual (pasos 2-4)
ViridianForest_EventScript_CuboneClue::
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 1, ViridianForest_EventScript_CuboneClue2
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 2, ViridianForest_EventScript_CuboneClue3
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 3, ViridianForest_EventScript_CuboneClue4
	return

ViridianForest_EventScript_CuboneClue2::
	msgbox ViridianForest_Text_CuboneClue2, MSGBOX_DEFAULT
	return

ViridianForest_EventScript_CuboneClue3::
	msgbox ViridianForest_Text_CuboneClue3, MSGBOX_DEFAULT
	return

ViridianForest_EventScript_CuboneClue4::
	msgbox ViridianForest_Text_CuboneClue4, MSGBOX_DEFAULT
	return

ViridianForest_EventScript_SparkleNoCubone::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox ViridianForest_Text_SomethingGlitters, MSGBOX_DEFAULT
	releaseall
	end

ViridianForest_EventScript_SparkleSkip::
	releaseall
	end

ViridianForest_EventScript_CuboneSniff1::
	msgbox ViridianForest_Text_CuboneSniff1, MSGBOX_DEFAULT
	return

ViridianForest_EventScript_CuboneSniff2::
	msgbox ViridianForest_Text_CuboneSniff2, MSGBOX_DEFAULT
	return

ViridianForest_EventScript_CuboneSniff3::
	msgbox ViridianForest_Text_CuboneSniff3, MSGBOX_DEFAULT
	return

ViridianForest_EventScript_CuboneSniff4::
	msgbox ViridianForest_Text_CuboneSniff4, MSGBOX_DEFAULT
	return

ViridianForest_EventScript_CuboneSniff5::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox ViridianForest_Text_CuboneSniff5, MSGBOX_DEFAULT
	return

@ Coloca el brillo en la posición del paso actual del rastro.
@ removeobject activa el flag de ocultación; hay que limpiarlo para que el brillo
@ se vuelva a crear al entrar en pantalla (los pasos 2-5 nacen fuera de cámara).
ViridianForest_EventScript_PlaceSparkle::
	clearflag FLAG_HIDE_FOREST_MOSS_SPARKLE
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 0, ViridianForest_EventScript_PlaceSparkle1
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 1, ViridianForest_EventScript_PlaceSparkle2
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 2, ViridianForest_EventScript_PlaceSparkle3
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 3, ViridianForest_EventScript_PlaceSparkle4
	call_if_eq VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 4, ViridianForest_EventScript_PlaceSparkle5
	return

ViridianForest_EventScript_PlaceSparkle1::
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_SPARKLE, 15, 25
	return

ViridianForest_EventScript_PlaceSparkle2::
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_SPARKLE, 31, 12
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_DECOY1, 28, 18
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_DECOY2, 36, 16
	return

ViridianForest_EventScript_PlaceSparkle3::
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_SPARKLE, 47, 61
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_DECOY1, 45, 57
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_DECOY2, 50, 56
	return

ViridianForest_EventScript_PlaceSparkle4::
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_SPARKLE, 11, 55
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_DECOY1, 5, 60
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_DECOY2, 17, 56
	return

ViridianForest_EventScript_PlaceSparkle5::
	setobjectxyperm LOCALID_VIRIDIAN_FOREST_SPARKLE, 16, 39
	return

@ Etapa 3: el ambiente se oscurece y aparece Mew (al entrar al claro por el sur)
ViridianForest_EventScript_MewDarknessTrigger::
	lockall
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 4
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
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 5
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
	@ Agatha y sus soldados llegan en fila desde fuera de la pantalla, subiendo por el pasillo sur
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
	turnobject LOCALID_PLAYER, DIR_SOUTH
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

@ Los tres suben en fila por el pasillo sur (x16) y se abren al llegar al claro.
@ Agatha (16,40) -> (19,33), Soldado 1 (16,41) -> (15,35), Soldado 2 (16,42) -> (17,35)
ViridianForest_Movement_AgathaApproach::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_up
	face_right
	step_end

ViridianForest_Movement_Grunt1Approach::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	face_right
	step_end

ViridianForest_Movement_Grunt2Approach::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	face_right
	step_end

@ Soldado 1 se coloca justo debajo del jugador (20,34), rodeando por la fila 34
ViridianForest_Movement_GruntConfiscate::
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	face_up
	step_end

@ Soldado 2 se coloca a la izquierda de Agatha (18,33)
ViridianForest_Movement_GruntReportToAgatha::
	walk_up
	walk_right
	walk_up
	face_right
	step_end

ViridianForest_Movement_SetInvisible::
	set_invisible
	step_end
