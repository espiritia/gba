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

@ Dice que sí: Brock lo contrata, le da el traje y le explica el programa del Alto Mando
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
	msgbox SecretGym_Text_GetPokedex
	closemessage
	setvar VAR_PEWTER_CAVE_STATE, 4
	fadedefaultbgm
	releaseall
	end

SecretGym_EventScript_Brock::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_MALE
	goto_if_eq VAR_PEWTER_CAVE_STATE, 3, SecretGym_EventScript_BrockAskAgain
	msgbox SecretGym_Text_GoSeeOak
	release
	end

SecretGym_EventScript_BrockAskAgain::
	msgbox SecretGym_Text_ThoughtAboutIt
	goto SecretGym_EventScript_AskDeal
	end
