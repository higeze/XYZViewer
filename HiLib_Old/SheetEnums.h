#pragma once


/**
 *  Sort
 *  Sort state
 */
enum class Sorts
{
	None,
	Up,
	Down
};


/**
 *  Compare
 *  Compare state
 */
enum class Compares{
	Same,
	Diff,
	DiffNE
};

enum class Updates{
	None,
	RowVisible,
	ColumnVisible,
	Column,
	Row,
//	ColumnWidth,
//	ColumnFitWidth,
//	RowHeight,
//	RowFitHeight,
//	RowTop,
//	ColumnLeft,
	Scrolls,
	Invalidate,
	All
};