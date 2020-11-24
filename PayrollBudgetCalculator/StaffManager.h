#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "Staff.h"

#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

class StaffManager {
	std::vector<SalariedStaff> _vecSalariedStaff;
	std::vector<ContractStaff> _vecContractStaff;
public:
	void AddSalariedStaff(SalariedStaff objSalariedStaff);
	void AddContractStaff(ContractStaff objContractStaff);

	SalariedStaff* GetSalariedStaff(std::string strFullName);
	ContractStaff* GetContractStaff(std::string strFullName);

	std::vector<SalariedStaff>* GetPtrSalariedStaff() { return &_vecSalariedStaff; }
	std::vector<ContractStaff>* GetPtrContractStaff() { return &_vecContractStaff; }

	size_t CountSeniorStaff();
	size_t CountStandardStaff();
};

#endif // STAFFMANAGER_H
