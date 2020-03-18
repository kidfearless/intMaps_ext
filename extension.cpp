/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod IntMap Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "IHandleSys.h"
#include "smsdk_ext.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>


HandleType_t g_IntMapType = 0;


/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

IntMapHandler g_IntMap; /**< Global singleton for extension's main interface */

const sp_nativeinfo_t g_IntMapNatives[] = 
{
	{"IntMap.IntMap",					Native_IntMapIntMap},

	{"IntMap.SetValue",					Native_IntMap_SetValue},
	{"IntMap.SetArray",					Native_IntMap_SetArray},
	{"IntMap.SetString",				Native_IntMap_SetString},

	{"IntMap.GetValue",					Native_IntMap_GetValue},
	{"IntMap.GetArray",					Native_IntMap_GetArray},
	{"IntMap.GetString",				Native_IntMap_GetString},

	{"IntMap.RemoveCell",				Native_IntMap_RemoveCell},
	{"IntMap.RemoveString",				Native_IntMap_RemoveString},
	{"IntMap.RemoveArray",				Native_IntMap_RemoveArray},

	{"IntMap.RemoveAll",				Native_IntMap_RemoveAll},

	{"IntMap.ClearCells",				Native_IntMap_ClearCells},
	{"IntMap.ClearArrays",				Native_IntMap_ClearArrays},
	{"IntMap.ClearStrings",				Native_IntMap_ClearStrings},

	{"IntMap.ClearAll",					Native_IntMap_ClearAll},

	{"IntMap.HasCells",					Native_IntMap_HasCells},
	{"IntMap.HasArrays",				Native_IntMap_HasArrays},
	{"IntMap.HasStrings",				Native_IntMap_HasStrings},

	{"IntMap.Size.get",					Native_IntMap_SizeGet},
	{"IntMap.CellSize.get",				Native_IntMap_CellSizeGet},
	{"IntMap.StringSize.get",			Native_IntMap_StringSizeGet},
	{"IntMap.ArraySize.get",			Native_IntMap_ArraySizeGet},

	{nullptr,							nullptr},
};

bool IntMapHandler::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
	HandleError err;
	g_IntMapType = handlesys->CreateType("IntMap", this, 0, nullptr, nullptr, myself->GetIdentity(), &err);

	if (g_IntMapType == 0)
	{
		snprintf(error, maxlen, "Could not create IntMap (err: %d)", err);
	}

	sharesys->AddNatives(myself, g_IntMapNatives);
	sharesys->RegisterLibrary(myself, "intmap_ext");

	return true;
}

void IntMapHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	if (type == g_IntMapType)
	{
		delete (IntMap*) object;
	}
}

cell_t IntMapHandler::CreateHandle(IPluginContext *const pContext)
{
	auto intmap = new IntMap();

	HandleError handleError;
	auto handle = handlesys->CreateHandle(g_IntMapType, intmap, pContext->GetIdentity(), myself->GetIdentity(), &handleError);

	if (handle == BAD_HANDLE)
	{
		delete intmap;
		return pContext->ThrowNativeError("Cannot create IntMap (err: %d)", handleError);
	}

	return static_cast<cell_t>(handle);
}

void IntMapHandler::ReadHandle(IPluginContext *const pContext, const cell_t *params, pIntMap_t *value)
{
	HandleSecurity security(pContext->GetIdentity(), myself->GetIdentity());

	auto error = handlesys->ReadHandle(static_cast<Handle_t>(params[1]), g_IntMapType, &security, reinterpret_cast<void**>(value));

	if (error != HandleError_None)
	{
		pContext->ThrowNativeError("Error with reading IntMap Handle (err: %d)", error);
	} 
}

cell_t Native_IntMapIntMap(IPluginContext *pContext, const cell_t *params)
{
	return IntMapHandler::CreateHandle(pContext);
}

cell_t Native_IntMap_SetValue(IPluginContext *pContext, const cell_t *params)
{
	pIntMap_t value;
	IntMapHandler::ReadHandle(pContext, params, &value);
	return 0;
}

cell_t Native_IntMap_SetArray(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_SetString(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_GetValue(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_GetArray(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_GetString(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_RemoveCell(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_RemoveString(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_RemoveArray(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_RemoveAll(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_ClearCells(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_ClearArrays(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_ClearStrings(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_ClearAll(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_HasCells(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_HasArrays(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_HasStrings(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_SizeGet(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_CellSizeGet(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_StringSizeGet(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}

cell_t Native_IntMap_ArraySizeGet(IPluginContext *pContext, const cell_t *params)
{
	return 0;
}


SMEXT_LINK(&g_IntMap);
