#if defined _intmaps_included
 #endinput
#endif
#define _intmaps_included


// IntMaps unlike the stringmaps can store arrays, strings, and cells in separate containers.
// They are also stored in ordered maps allowing them to be iterated through
// at any point.
/*
	class IntMap
	{
		std::map<cell_t, std::string> tsrings;
		std::map<cell_t, cell_t> cells;
		std::map<cell_t, std::vector<cell_t>> multicells;
	}
*/

typedef LoopCellsCallback = function Action(IntMap map, int key, any value, any data);
typedef LoopArraysCallback = function Action(IntMap map, int key, const any[] values, int size, any data);
typedef LoopStringsCallback = function Action(IntMap map, int key, const char[] values, int size, any data);

methodmap IntMap < Handle
{
	public native IntMap();

	public native void SetValue(const int key, any value);
	public native any  GetValue(const int key);

	public native void SetArray(const int key, any[] value, int maxlength);
	public native void GetArray(const int key, any[] value, int maxlength);
	public native any  GetArrayCell(const int key, int index);
	public native int  GetArrayLength(const int key);

	public native void SetString(const int key, const char[] value);
	public native void GetString(const int key, char[] value, int maxlength);
	public native int  GetStringLength(const int key);

	public native void RemoveCell(const int key);
	public native void RemoveString(const int key);
	public native void RemoveArray(const int key);

	public native void RemoveAll(const int key);

	public native void ClearCells();
	public native void ClearArrays();
	public native void ClearStrings();
	public native void ClearAll();

	public native bool HasValue(const int key);
	public native bool HasArray(const int key);
	public native bool HasString(const int key);

	public native void IterateCells(LoopCellsCallback func, any data = 0);

	// warning: arrays have to be recreated on the heap for each iteration
	public native void IterateArrays(LoopArraysCallback func, any data = 0);
	
	public native void IterateStrings(LoopStringsCallback func, any data = 0);


	property int CellSize
	{
		public native get();
	}

	property int StringSize
	{
		public native get();
	}
	
	property int ArraySize
	{
		public native get();
	}
};