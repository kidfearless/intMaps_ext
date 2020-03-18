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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

/**
 * @file extension.h
 * @brief IntMap extension code header.
 */

#include "IHandleSys.h"
#include "smsdk_ext.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>

class IntMap
{
private:
	std::map<cell_t, std::string> strings;
	std::map<cell_t, cell_t> cells;
	std::map<cell_t, std::vector<cell_t>> multicells;

public:
	IntMap()
	{
	}

	~IntMap()
	{
	}

	// Sets a value in a hash map, either inserting a new entry or replacing an old one.
	void SetValue(const cell_t key, cell_t value)
	{
		this->cells[key] = value;
	}

	// Sets an array value in a Map, either inserting a new entry or replacing an old one.
	void SetArray(const cell_t key, cell_t arr[], cell_t num_items)
	{
		std::vector<cell_t> vector;
		for (int i = 0; i < num_items; ++i)
		{
			vector[i] = arr[i];
		}
		this->multicells[key] = vector;
	}

	// Sets a string value in a Map, either inserting a new entry or replacing an old one.
	void SetString(const cell_t key, const char *value)
	{
		std::string string(value);
		this->strings[key] = value;
	}

	// Retrieves a value in a Map.
	cell_t GetValue(const cell_t key)
	{
		return this->cells[key];
	}

	// Retrieves an array in a Map.
	std::vector<cell_t> GetArray(const cell_t key)
	{
		return this->multicells[key];
	}

	// Retrieves a string in a Map.
	std::string GetString(const cell_t key)
	{
		return this->strings[key];
	}

	// Removes a key entry from a Map.
	void RemoveCell(const cell_t key)
	{
		auto index = this->cells.find(key);
		this->cells.erase(index);
	}

	void RemoveString(const cell_t key)
	{
		auto index = this->strings.find(key);
		this->strings.erase(index);
	}

	void RemoveArray(const cell_t key)
	{
		auto index = this->multicells.find(key);
		this->multicells.erase(index);
	}

	void RemoveAll(const cell_t key)
	{
		this->RemoveCell(key);
		this->RemoveString(key);
		this->RemoveArray(key);
	}

	// Clears all entries from a Map.
	void ClearCells()
	{
	}

	bool HasValue(const cell_t key)
	{
		return true;
	}
};

using pIntMap_t = IntMap*;

/**
 * @brief IntMap implementation of the SDK Extension.
 * Note: Uncomment one of the pre-defined virtual functions in order to use it.
 */
class IntMapHandler : public SDKExtension, public IHandleTypeDispatch
{
public:
    // IHandleTypeDispatch::OnHandleDestroy
    virtual void OnHandleDestroy(HandleType_t type, void *object);

public:
	/**
	 * @brief This is called after the initial loading sequence has been processed.
	 *
	 * @param error		Error message buffer.
	 * @param maxlen	Size of error message buffer.
	 * @param late		Whether or not the module was loaded after map load.
	 * @return			True to succeed loading, false to fail.
	 */
	virtual bool SDK_OnLoad(char *error, size_t maxlen, bool late);
	
	/**
	 * @brief This is called right before the extension is unloaded.
	 */
	//virtual void SDK_OnUnload();

	/**
	 * @brief This is called once all known extensions have been loaded.
	 * Note: It is is a good idea to add natives here, if any are provided.
	 */
	//virtual void SDK_OnAllLoaded();

	/**
	 * @brief Called when the pause state is changed.
	 */
	//virtual void SDK_OnPauseChange(bool paused);

	/**
	 * @brief this is called when Core wants to know if your extension is working.
	 *
	 * @param error		Error message buffer.
	 * @param maxlen	Size of error message buffer.
	 * @return			True if working, false otherwise.
	 */
	//virtual bool QueryRunning(char *error, size_t maxlen);
public:
#if defined SMEXT_CONF_METAMOD
	/**
	 * @brief Called when Metamod is attached, before the extension version is called.
	 *
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @param late			Whether or not Metamod considers this a late load.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late);

	/**
	 * @brief Called when Metamod is detaching, after the extension version is called.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodUnload(char *error, size_t maxlen);

	/**
	 * @brief Called when Metamod's pause state is changing.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param paused		Pause state being set.
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodPauseChange(bool paused, char *error, size_t maxlen);
#endif

public:
	static cell_t CreateHandle(IPluginContext *const pContext);
	static void ReadHandle(IPluginContext *const pContext, const cell_t *params, pIntMap_t *value);
}extern g_IntMap;

/**
 * IntMap natives
 */

cell_t Native_IntMapIntMap(IPluginContext *pContext, const cell_t *params);

cell_t Native_IntMap_SetValue(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_SetArray(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_SetString(IPluginContext *pContext, const cell_t *params);

cell_t Native_IntMap_GetValue(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_GetArray(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_GetString(IPluginContext *pContext, const cell_t *params);

cell_t Native_IntMap_RemoveCell(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_RemoveString(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_RemoveArray(IPluginContext *pContext, const cell_t *params);

cell_t Native_IntMap_RemoveAll(IPluginContext *pContext, const cell_t *params);

cell_t Native_IntMap_ClearCells(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_ClearArrays(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_ClearStrings(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_ClearAll(IPluginContext *pContext, const cell_t *params);

cell_t Native_IntMap_HasCells(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_HasArrays(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_HasStrings(IPluginContext *pContext, const cell_t *params);

cell_t Native_IntMap_SizeGet(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_CellSizeGet(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_StringSizeGet(IPluginContext *pContext, const cell_t *params);
cell_t Native_IntMap_ArraySizeGet(IPluginContext *pContext, const cell_t *params);




extern HandleType_t g_IntMapType;

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
