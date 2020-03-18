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
	{nullptr,					nullptr},
};

bool IntMapHandler::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
	HandleError err;
	g_IntMapType = handlesys->CreateType("IntMap", this, 0, nullptr, nullptr, myself->GetIdentity(), &err);

	if (g_IntMapType == 0)
	{
		snprintf(error, maxlen, "Could not create Double handle type (err: %d)", err);
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

cell_t IntMapHandler::CreateHandle(IPluginContext *const pContext, pIntMap_t value)
{
	auto intmap = new IntMap();

	if (value)
	{
		*intmap = *value;
	}

	HandleError handleError;
	auto handle = handlesys->CreateHandle(g_IntMapType, intmap, pContext->GetIdentity(), myself->GetIdentity(), &handleError);

	if (handle == BAD_HANDLE)
	{
		delete intmap;
		return pContext->ThrowNativeError("Cannot create double handle (err: %d)", handleError);
	}

	return static_cast<cell_t>(handle);
}

HandleError IntMapHandler::ReadHandle(IPluginContext *const pContext, const Handle_t &handle, pIntMap_t *value)
{
	HandleSecurity security(pContext->GetIdentity(), myself->GetIdentity());

	return handlesys->ReadHandle(handle, g_IntMapType, &security, reinterpret_cast<void**>(value));
}

/* cell_t native_DoubleConstructor(IPluginContext *pContext, const cell_t *)
{
	return IntMap::CreateHandle(pContext);
}

cell_t native_DoubleFromString(IPluginContext *pContext, const cell_t *params)
{
	char *str;
	pContext->LocalToString(params[1], &str);

	auto value = std::atof(str);

	return IntMap::CreateHandle(pContext, &value);
}

cell_t native_DoubleFromFloat(IPluginContext *pContext, const cell_t *params)
{
	auto value = static_cast<double>(sp_ctof(params[1]));
	
	return IntMap::CreateHandle(pContext, &value);
}

cell_t native_DoubleFromInt(IPluginContext *pContext, const cell_t *params)
{
	auto value = static_cast<double>(params[1]);

	return IntMap::CreateHandle(pContext, &value);
} */

SMEXT_LINK(&g_IntMap);
