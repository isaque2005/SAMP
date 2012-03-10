PlayRadioStation(playerID, id)
{
	if (id)
	{
		PlayAudioStreamForPlayer(playerID, GetMySQLValue("radiostations", "URL", "ID", val2str(id)));
		new vehicleID = GetPlayerVehicleID(playerID);
		if (vehicles[vehicleID][VEHICLE_SAVEID])
		{
			vehicles[vehicleID][VEHICLE_RADIOSTATION] = id;
			SetMySQLValue("savedvehicles", "LastRadioStation", val2str(id), "ID", val2str(vehicles[vehicleID][VEHICLE_RADIOSTATION]));
		}
		TextDrawSetString(textDrawRadioStation[playerID], GetMySQLValue("radiostations", "Name", "ID", val2str(id)));
		TextDrawShowForPlayer(playerID, textDrawRadioStation[playerID]);
		KillTimer(PVar:playerID[RADIODISPLAYTIMER]);
		PVar:playerID[RADIODISPLAYTIMER] = SetTimerEx("Timer_HideRadioDisplay", TIMER_HIDERADIODISPLAY, false, "i", playerID);
	}
	else
	{
		SendClientMessage(playerID, COLOR_ERROR, "Ung�ltiger Radiosender!");
	}
}