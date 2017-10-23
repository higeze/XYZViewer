#pragma once

namespace Operation{
	enum Type{
		None,
		HeaderTracking,
		HeaderDraging,
		CellEditing
	};
}

typedef std::shared_ptr<Operation::Type> OperationPtr;