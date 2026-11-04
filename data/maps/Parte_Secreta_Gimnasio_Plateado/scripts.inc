Parte_Secreta_Gimnasio_Plateado_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SecretGym_OnFrame
	.byte 0

@ Recién salido de la cueva: Brock espera debajo del jugador y le propone el trato
SecretGym_OnFrame::
	map_script_2 VAR_PEWTER_CAVE_STATE, 2, SecretGym_EventScript_BrockOffer
	.2byte 0

SecretGym_EventScript_BrockOffer::
	lockall
	playbgm MUS_FOLLOW_ME, FALSE
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_SECRET_GYM_BROCK, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SecretGym_Text_BrockOffer
SecretGym_EventScript_AskDeal::
	msgbox SecretGym_Text_WhatDoYouSay, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SecretGym_EventScript_DealAccepted
	goto SecretGym_EventScript_DealRejected
	end

@ Dice que no: Brock se queda esperando aquí abajo y el ascensor sube a la entrada del gimnasio
SecretGym_EventScript_DealRejected::
	msgbox SecretGym_Text_Rejected
	closemessage
	setvar VAR_PEWTER_CAVE_STATE, 3
	fadedefaultbgm
	releaseall
	end

@ Dice que sí: Brock lo contrata, le da el traje, le explica el programa del Alto Mando
@ y se va por el ascensor; el jugador debe inscribirse con el PROF. PIZARRA
SecretGym_EventScript_DealAccepted::
	msgbox SecretGym_Text_Hired
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_KEY_ITEM
	message SecretGym_Text_ReceivedUniform
	waitmessage
	waitfanfare
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_PLAYER_GYM_UNIFORM
	special UpdatePlayerAvatarOutfit
	delay 30
	fadescreen FADE_FROM_BLACK
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SecretGym_Text_LooksGood
	msgbox SecretGym_Text_EliteFourProgram
	msgbox SecretGym_Text_RegisterWithScientist
	closemessage
	applymovement LOCALID_SECRET_GYM_BROCK, SecretGym_Movement_BrockToElevator
	waitmovement 0
	playse SE_EXIT
	removeobject LOCALID_SECRET_GYM_BROCK
	setflag FLAG_HIDE_SECRET_GYM_BROCK
	setvar VAR_PEWTER_CAVE_STATE, 4
	fadedefaultbgm
	releaseall
	end

@ (5,4) -> (13,3), delante del ascensor
SecretGym_Movement_BrockToElevator::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	face_up
	step_end

@ Solo está en la sala mientras el trato sigue pendiente (estado 3)
SecretGym_EventScript_Brock::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SecretGym_Text_ThoughtAboutIt
	goto SecretGym_EventScript_AskDeal
	end

@ PROF. PIZARRA: inscribe al jugador en el programa de la Liga y le da la POKÉDEX
SecretGym_EventScript_Scientist::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_MALE
	goto_if_eq VAR_PEWTER_CAVE_STATE, 4, SecretGym_EventScript_ScientistRegister
	goto_if_ge VAR_PEWTER_CAVE_STATE, 5, SecretGym_EventScript_ScientistBrockWaits
	msgbox SecretGym_Text_ScientistTalkToBrock
	release
	end

SecretGym_EventScript_ScientistRegister::
	msgbox SecretGym_Text_ScientistWelcome
	closemessage
	applymovement LOCALID_SECRET_GYM_SCIENTIST, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	playse SE_PC_LOGIN
	waitse
	delay 30
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message SecretGym_Text_Registering
	waitmessage
	playse SE_PC_ON
	waitse
	delay 30
	playse SE_PC_OFF
	waitse
	closemessage
	faceplayer
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SecretGym_Text_ScientistDone
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_KEY_ITEM
	message SecretGym_Text_ReceivedPokedex
	waitmessage
	waitfanfare
	setflag FLAG_SYS_POKEDEX_GET
	special SetUnlockedPokedexFlags
	setvar VAR_MAP_SCENE_POKEMON_CENTER_TEALA, 1
	setvar VAR_PEWTER_CAVE_STATE, 5
	textcolor NPC_TEXT_COLOR_MALE
	goto SecretGym_EventScript_ScientistBrockWaits
	end

SecretGym_EventScript_ScientistBrockWaits::
	msgbox SecretGym_Text_ScientistBrockWaits
	release
	end

@ La puerta de la cueva (5,2): la entrada se derrumbó en la huida, así que no se puede volver
SecretGym_EventScript_CaveBlocked::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox SecretGym_Text_CaveBlocked
	closemessage
	applymovement LOCALID_PLAYER, SecretGym_Movement_StepBackFromCave
	waitmovement 0
	releaseall
	end

SecretGym_Movement_StepBackFromCave::
	walk_down
	step_end
