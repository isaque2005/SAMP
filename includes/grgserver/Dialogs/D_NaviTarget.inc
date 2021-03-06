ShowDialog:NaviTarget(playerID)
{
	new targets[2048];
	AddLine(targets, LanguageString(playerID, StringID:173("Bank")));// Bank
	AddLine(targets, LanguageString(playerID, StringID:384("Gas station")));// Gas station
	AddLine(targets, LanguageString(playerID, StringID:411("Hospital")));// Hospital
	AddLine(targets, LanguageString(playerID, StringID:174("Hotel")));// Hotel
	AddLine(targets, LanguageString(playerID, StringID:175("House")));// House
	AddLine(targets, LanguageString(playerID, StringID:437("Vehicle upgrader")));// Vehicle upgrader
	MySQLQuery("SELECT `name` FROM `navigationlocationtypes` ORDER BY `name` ASC");
	mysql_store_result();
	while (FetchMySQLRow())
	{
		new string[256];
		strcat(string, GetMySQLField("name"));
		new checkCharacter;
		new languageStringID;
		if (!sscanf(string, "p<:>cd", checkCharacter, languageStringID))
		{
			if (checkCharacter == 'L')
			{
				Format:string(LanguageString(playerID, languageStringID));
			}
		}
		AddLine(targets, string);
	}
	mysql_free_result();
	AddLine(targets, LanguageString(playerID, StringID:342("Target on map")));// Target on map
	AddLine(targets, LanguageString(playerID, StringID:417("Name of a property")));// Name of a property
	CreateDialog(playerID, "NaviTarget", DIALOG_STYLE_LIST, LanguageString(playerID, StringID:247("Select target")), targets, "OK", LanguageString(playerID, StringID:165("Cancel")));
}

DialogResponse:NaviTarget(playerID, response, listItem, inputText[])
{
	if (!response)
	{
		return false;
	}
	Count(index, MAX_DISTANCECALCELEMENTS)
	{
		distanceCalcArray[index][DISTANCECALC_DISTANCE] = 6000.0;
	}
	new pickupType[STRINGLENGTH_PICKUPTYPE];
	if (IsStr(inputText, LanguageString(playerID, StringID:173("Bank"))))// Bank
	{
		strcat(pickupType, "BANK");
	}
	if (IsStr(inputText, LanguageString(playerID, StringID:384("Gas station"))))// Gas station
	{
		strcat(pickupType, "GASSTATION");
	}
	if (IsStr(inputText, LanguageString(playerID, StringID:411("Hospital"))))// Hospital
	{
		strcat(pickupType, "HOSPITAL");
	}
	if (IsStr(inputText, LanguageString(playerID, StringID:174("Hotel"))))// Hotel
	{
		strcat(pickupType, "HOTEL");
	}
	if (IsStr(inputText, LanguageString(playerID, StringID:175("House"))))// House
	{
		strcat(pickupType, "HOUSE");
	}
	if (IsStr(inputText, LanguageString(playerID, StringID:437("Vehicle upgrader"))))// Vehicle upgrader
	{
		strcat(pickupType, "UPGRADEVEHICLE");
	}
	if (strlen(pickupType))
	{
		Count(pickupID, MAX_PICKUPS)
		{
			if (IsStr(pickups[pickupID][PICKUP_TYPE], pickupType))
			{
				distanceCalcArray[pickupID][DISTANCECALC_DISTANCE] = GetPlayerDistanceFromPoint(playerID, pickups[pickupID][PICKUP_POSX], pickups[pickupID][PICKUP_POSY], pickups[pickupID][PICKUP_POSZ]);
				distanceCalcArray[pickupID][DISTANCECALC_POSX] = pickups[pickupID][PICKUP_POSX];
				distanceCalcArray[pickupID][DISTANCECALC_POSY] = pickups[pickupID][PICKUP_POSY];
				distanceCalcArray[pickupID][DISTANCECALC_POSZ] = pickups[pickupID][PICKUP_POSZ];
			}
		}
		SortDeepArray(distanceCalcArray, DISTANCECALC_DISTANCE);
		Count(index, MAX_NAVITARGETS)
		{
			naviTargets[playerID][index][NAVITARGET_DISTANCE] = distanceCalcArray[index][DISTANCECALC_DISTANCE];
			naviTargets[playerID][index][NAVITARGET_POSX] = distanceCalcArray[index][DISTANCECALC_POSX];
			naviTargets[playerID][index][NAVITARGET_POSY] = distanceCalcArray[index][DISTANCECALC_POSY];
			naviTargets[playerID][index][NAVITARGET_POSZ] = distanceCalcArray[index][DISTANCECALC_POSZ];
		}
		ShowDialog:NaviTarget_TargetList(playerID);
		return true;
	}
	new Float:target[3];
	if (IsStr(inputText, LanguageString(playerID, StringID:342("Target on map"))))// Target on map
	{
		if (PVar:playerID[MAP_POSX] && PVar:playerID[MAP_POSY] && PVar:playerID[MAP_POSZ])
		{
			target[0] = PVar:playerID[MAP_POSX];
			target[1] = PVar:playerID[MAP_POSY];
			target[2] = PVar:playerID[MAP_POSZ];
		}
		else
		{
			SendClientMessageEx(playerID, COLOR_ERROR, StringID:264("You have not selected the target on your map!"));
			return false;
		}
	}
	if (IsStr(inputText, LanguageString(playerID, StringID:417("Name of a property"))))// Name of a property
	{
		ShowDialog:NaviTarget_PropertyName(playerID);
		return true;
	}
	if (!target[0] && !target[1] && !target[2])
	{
		new typeID = GetNavigationLocationTypeByName(playerID, inputText);
		if (typeID)
		{
			new index;
			MySQLQuery("SELECT * FROM `navigationlocations` WHERE `type` = '%d'", typeID);
			mysql_store_result();
			while (FetchMySQLRow())
			{
				distanceCalcArray[index][DISTANCECALC_DISTANCE]= GetPlayerDistanceFromPoint(playerID, floatstr(GetMySQLField("posX")), floatstr(GetMySQLField("posY")), floatstr(GetMySQLField("posZ")));
				distanceCalcArray[index][DISTANCECALC_POSX] = floatstr(GetMySQLField("posX"));
				distanceCalcArray[index][DISTANCECALC_POSY] = floatstr(GetMySQLField("posY"));
				distanceCalcArray[index][DISTANCECALC_POSZ] = floatstr(GetMySQLField("posZ"));
				index++;
				if (index >= MAX_DISTANCECALCELEMENTS)
				{
					break;
				}
			}
			mysql_free_result();
			SortDeepArray(distanceCalcArray, DISTANCECALC_DISTANCE);
			Count(targetIndex, MAX_NAVITARGETS)
			{
				naviTargets[playerID][targetIndex][NAVITARGET_DISTANCE] = distanceCalcArray[targetIndex][DISTANCECALC_DISTANCE];
				naviTargets[playerID][targetIndex][NAVITARGET_POSX] = distanceCalcArray[targetIndex][DISTANCECALC_POSX];
				naviTargets[playerID][targetIndex][NAVITARGET_POSY] = distanceCalcArray[targetIndex][DISTANCECALC_POSY];
				naviTargets[playerID][targetIndex][NAVITARGET_POSZ] = distanceCalcArray[targetIndex][DISTANCECALC_POSZ];
			}
			ShowDialog:NaviTarget_TargetList(playerID);
			return true;
		}
	}
	if (target[0] || target[1] || target[2])
	{
		if (CalculateRoute(playerID, target[0], target[1], target[2]))
		{
			SendClientMessageEx(playerID, COLOR_INFO, StringID:414("Calculating route"));
		}
		else
		{
			SendClientMessageEx(playerID, COLOR_ERROR, StringID:415("No GPS!"));
		}
	}
	else
	{
		SendClientMessageEx(playerID, COLOR_ERROR, StringID:343("No target found!"));
	}
	return true;
}