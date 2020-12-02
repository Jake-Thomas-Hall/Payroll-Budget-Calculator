#pragma once

#include "Menu.h"
#include "Utilities.h"
#include "InputValidator.h"

void MenuContainer::Execute()
{
	std::cout << _strText << "\n";
	util::for_each_iterator(_vecMenuItems.begin(), _vecMenuItems.end(), 0, [](int index, std::unique_ptr<MenuItem> const& item) {
		std::cout << "\t" << index << " - " << item->ItemText() << "\n";
		});
	std::cout << "\nChoice: ";
	int iChoice = InputValidator::ValidateInt();
	if ((int)_vecMenuItems.size() > iChoice && iChoice >= 0) {
		_vecMenuItems[iChoice]->Execute();
	}
	else {
		std::cout << "Not a valid option, please try again.\n";
	}
};

void MenuContainer::AddMenuItem(std::unique_ptr<MenuItem> item) {
	_vecMenuItems.push_back(std::move(item));
};

GeneralMenuItem::GeneralMenuItem(std::string output, StaffManager* staffManager)
{
	_output = output;
	_ptrStaffManager = staffManager;
};

void MenuStaffManagement::Execute() {
	MenuContainer objMenuContainer = MenuContainer("Staff management menu.\nChoose one of the below options.\n");
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Return to menu", &objMenuContainer)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuViewStaff("View staff", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuAddStaffMember("Add staff member", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuRemoveStaffMember("Remove staff member", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuUpdateStaff("Update staff member", _ptrStaffManager)));

	while (!objMenuContainer.GetExitMenu()) {
		system("cls");
		objMenuContainer.Execute();
	}
};

MenuExit::MenuExit(std::string output, MenuContainer* menuContainer) : GeneralMenuItem(output, NULL) {
	_ptrMenuContainer = menuContainer;
};

void MenuExit::Execute() {
	_ptrMenuContainer->SetExitMenu(true);
};

void MenuCalculateBudget::Execute() {
	MenuContainer objMenuContainer = MenuContainer("\nChoose one of the below options.\n");
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Return to menu", &objMenuContainer)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuViewPayrollBudgetReport("View payroll budget report", _ptrBudgetCalculator)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuUpdateMinOverrun("Set minimum budget overrun", _ptrBudgetCalculator)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuUpdateMaxOverrun("Set maximum budget overrun", _ptrBudgetCalculator)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuUpdateProjectLength("Set project length", _ptrBudgetCalculator)));

	while (!objMenuContainer.GetExitMenu()) {
		system("cls");
		std::cout << "Payroll budget calculation menu\n";
		std::cout << "\nCurrent project length: " << _ptrBudgetCalculator->GetProjectLength() << ((_ptrBudgetCalculator->GetProjectLength() <= 1) ? " year" : " years") << "\n";
		std::cout << "Budget overrun: " << _ptrBudgetCalculator->GetMinOverPercent() << "% - " << _ptrBudgetCalculator->GetMaxOverPercent() << "%\n";
		if (_ptrStaffManager->CountSeniorStaff() < 3) std::cout << "WARNING: You currently only have " << _ptrStaffManager->CountSeniorStaff() <<
			" senior staff, this is below the recommended 3 senior staff for a project.\n";

		if (_ptrStaffManager->CountStandardStaff() < 5) std::cout << "WARNING: You currently only have " << _ptrStaffManager->CountStandardStaff() <<
			" salaried staff, this is below the recommended 5 salaried staff for a project.\n";

		if (_ptrStaffManager->CountContractStaff() < 5) std::cout << "WARNING: You currently only have " << _ptrStaffManager->CountContractStaff() <<
			" contract staff, this is below the recommended 5 contracted staff for a project.\n";
		objMenuContainer.Execute();
	}
};

void MenuSaveLoad::Execute() {
	system("cls");
	std::cout << "Save or Load menu\nThis is a placeholder menu option...\n";
	util::Pause();
};

void MenuViewStaff::Execute() {
	MenuContainer objMenuContainer = MenuContainer("");
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Return to staff management menu", &objMenuContainer)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuAddStaffMember("Add staff member", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuRemoveStaffMember("Remove staff member", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuUpdateStaff("Update staff member", _ptrStaffManager)));

	while (!objMenuContainer.GetExitMenu()) {
		system("cls");

		std::vector<SalariedStaff>* ptrVecSalariedStaff = _ptrStaffManager->GetPtrSalariedStaff();
		std::vector<ContractStaff>* ptrVecContractStaff = _ptrStaffManager->GetPtrContractStaff();

		std::cout << "View staff\n\n";

		if (ptrVecSalariedStaff->size() < 1 && ptrVecContractStaff->size() < 1) {
			std::cout << "There are not currently any staff in the system. Either add some or import them to use this menu.\n";
			objMenuContainer.SetExitMenu(true);
			util::Pause();
			continue;
		}

		if (ptrVecSalariedStaff->size() > 0) {
			std::cout << "Salaried Staff\n";
			std::cout << "Count: " << ptrVecSalariedStaff->size() << "\n";
			util::OutputSalariedStaffHeader();
			util::OutputSalariedStaff(ptrVecSalariedStaff);
			std::cout << "\n";

			if (_ptrStaffManager->CountSeniorStaff() < 3) std::cout << "Warning: You currently have less than 3 senior staff members. A normal project should contain at least 3 senior staff.\n";
			if (_ptrStaffManager->CountStandardStaff() < 5) std::cout << "Warning: You currently have less than 5 salaried staff members. A normal project should contain at least 5 salaried staff.\n";

			std::cout << "\n";
		}
		else {
			std::cout << "There are not currently any salaried staff to list.\nPlease note a 'normal' project usually has 3 senior staff and 5 salaried staff.\n";
		}

		if (ptrVecContractStaff->size() > 0) {
			std::cout << "Contract Staff\n";
			std::cout << "Count: " << ptrVecContractStaff->size() << "\n";
			util::OutputContractStaffHeader();
			util::OutputContractStaff(ptrVecContractStaff);
			std::cout << "\n";

			if (ptrVecContractStaff->size() < 5) std::cout << "Warning: You currently have less than 5 contracted staff. A normal project should contain at least 5 contracted staff.\n";
		}
		else {
			std::cout << "There are not currently any contract staff to list.\nPlease note a 'normal' project usually has 5 contract staff.\n";
		}

		objMenuContainer.Execute();
	}
}

void SubMenuAddStaffMember::Execute() {
	MenuContainer objMenuContainer = MenuContainer("Choose Staff type to add.\n");
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Return to previous", &objMenuContainer)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuAddSalariedStaff("Add salaried staff member (including senior)", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuAddContractStaff("Add contract staff member", _ptrStaffManager)));

	while (!objMenuContainer.GetExitMenu()) {
		system("cls");
		objMenuContainer.Execute();
	}
};

void SubMenuAddSalariedStaff::Execute() {
	system("cls");
	std::cout << "Add Salaried Staff\n";

	std::string strFirstName, strLastName, strJobRole, strDepartment;
	int iSeniorStatus;
	SalariedStaff objSalariedStaff;

	std::cout << "First Name: ";
	strFirstName = InputValidator::ValidateString(15);
	std::cout << "Last Name: ";
	strLastName = InputValidator::ValidateString(15);
	std::cout << "Job Role: ";
	strJobRole = InputValidator::ValidateString(25);
	std::cout << "Department: ";
	strDepartment = InputValidator::ValidateString(20);

	objSalariedStaff.UpdateDetails(strFirstName, strLastName, strJobRole, strDepartment);

	std::cout << "Salary: ";
	objSalariedStaff.SetSalary(InputValidator::ValidateDouble());
	std::cout << "Senior (1 for senior, or 0 otherwise): ";
	iSeniorStatus = InputValidator::ValidateInt();

	if (iSeniorStatus == 1) {
		objSalariedStaff.SetSenior(true);
	}

	try {
		_ptrStaffManager->AddSalariedStaff(objSalariedStaff);
		std::cout << "\nStaff member " << objSalariedStaff.GetFullName() << " added.\n\n";
		util::Pause();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n\n";
	}
};

void SubMenuAddContractStaff::Execute() {
	system("cls");
	std::cout << "Add Salaried Staff\n";

	std::string strFirstName, strLastName, strJobRole, strDepartment;
	ContractStaff objContractStaff;

	std::cout << "First Name: ";
	strFirstName = InputValidator::ValidateString(15);
	std::cout << "Last Name: ";
	strLastName = InputValidator::ValidateString(15);
	std::cout << "Job Role: ";
	strJobRole = InputValidator::ValidateString(25);
	std::cout << "Department: ";
	strDepartment = InputValidator::ValidateString(20);

	objContractStaff.UpdateDetails(strFirstName, strLastName, strJobRole, strDepartment);

	std::cout << "Wage (per hour): ";
	objContractStaff.SetWage(InputValidator::ValidateDouble());
	std::cout << "Weekly hours (max 20h): ";
	objContractStaff.SetWeeklyHours(InputValidator::ValidateDouble(20));
	std::cout << "Contracted Weeks (max 30 weeks): ";
	objContractStaff.SetWeeks(InputValidator::ValidateInt(30));

	try {
		_ptrStaffManager->AddContractStaff(objContractStaff);
		std::cout << "\nContract staff member " << objContractStaff.GetFullName() << " added.\n\n";
		util::Pause();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n\n";
		util::Pause();
	}
};

void SubMenuRemoveStaffMember::Execute() {
	MenuContainer objMenuContainer = MenuContainer("Choose Staff type to add.\n");
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Return to previous", &objMenuContainer)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuRemoveSalariedStaffMember("Remove salaried staff member (including senior)", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuRemoveContractStaff("Remove contract staff member", _ptrStaffManager)));

	while (!objMenuContainer.GetExitMenu()) {
		system("cls");
		objMenuContainer.Execute();
	}
};

void SubMenuRemoveSalariedStaffMember::Execute() {
	bool boolExitWhile = false;
	do {
		system("cls");
		std::cout << "Remove Salaried staff menu\n\n";

		std::vector<SalariedStaff>* ptrVecSalariedStaff = _ptrStaffManager->GetPtrSalariedStaff();

		if (ptrVecSalariedStaff->size() < 1) {
			std::cout << "No salaried staff found.\n";
			boolExitWhile = true;
			util::Pause();
		}
		else {
			util::OutputSalariedStaffHeader();
			util::OutputSalariedStaff(ptrVecSalariedStaff);
			std::cout << "\n";

			std::cout << "Note: Enter 0 to exit.\n";
			std::cout << "Full name of salaried staff to remove: ";
			std::string strFullName = InputValidator::ValidateString();

			if (strFullName == "0") {
				boolExitWhile = true;
			}
			else {
				try {
					_ptrStaffManager->RemoveSalariedStaff(strFullName);
				}
				catch (std::exception& ex) {
					std::cout << ex.what() << "\n";
					util::Pause();
				}
			}
		}
	} while (!boolExitWhile);
};

void SubMenuRemoveContractStaff::Execute() {
	bool boolExitWhile = false;
	do {
		system("cls");
		std::cout << "Remove contract staff menu\n\n";

		std::vector<ContractStaff>* ptrVecContractStaff = _ptrStaffManager->GetPtrContractStaff();

		if (ptrVecContractStaff->size() < 1) {
			std::cout << "No contract staff found.\n";
			boolExitWhile = true;
			util::Pause();
		}
		else {
			util::OutputContractStaffHeader();
			util::OutputContractStaff(ptrVecContractStaff);
			std::cout << "\n";

			std::cout << "Note: Enter 0 to exit.\n";
			std::cout << "Full name of contract staff to remove: ";
			std::string strFullName = InputValidator::ValidateString();

			if (strFullName == "0") {
				boolExitWhile = true;
			}
			else {
				try {
					_ptrStaffManager->RemoveContractStaff(strFullName);
				}
				catch (std::exception& ex) {
					std::cout << ex.what() << "\n";
					util::Pause();
				}
			}
		}
	} while (!boolExitWhile);
};

void MenuUpdateStaff::Execute() {
	MenuContainer objMenuContainer = MenuContainer("Choose Staff type to update.\n");
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Return to previous", &objMenuContainer)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuUpdateSalariedStaff("Update salaried staff members (including senior)", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuUpdateContractStaff("Update contract staff members", _ptrStaffManager)));

	while (!objMenuContainer.GetExitMenu()) {
		system("cls");
		objMenuContainer.Execute();
	}
}

void SubMenuUpdateSalariedStaff::Execute() {
	bool boolExitWhile = false;
	do {
		system("cls");
		std::cout << "Current salaried staff\n\n";

		if (_ptrVecSalariedStaff->size() < 1) {
			std::cout << "There are not currently any salaried staff in the system. To update staff details, you first must add at least one staff member\n";
			boolExitWhile = true;
			util::Pause();
			continue;
		}

		util::OutputSalariedStaffHeader();
		util::OutputSalariedStaff(_ptrVecSalariedStaff);

		std::cout << "\nPlease enter the full name of the staff member you would like to update.\nNote: Enter 0 to exit this update screen and return.\n";
		std::cout << "Full name: ";
		std::string strFullName = InputValidator::ValidateString();

		if (strFullName == "0") {
			boolExitWhile = true;
			continue;
		}

		SalariedStaff* ptrSalariedStaff = _ptrStaffManager->GetSalariedStaff(strFullName);

		while (ptrSalariedStaff == NULL) {
			std::cout << "No staff member with this name found, please try again (enter 0 to cancel).\n";
			std::cout << "Full name: ";
			strFullName = InputValidator::ValidateString();
			if (strFullName == "0") {
				boolExitWhile = true;
				ptrSalariedStaff = new SalariedStaff();
				continue;
			}
			ptrSalariedStaff = _ptrStaffManager->GetSalariedStaff(strFullName);
		}

		MenuContainer objMenuContainer = MenuContainer("Choose action.\n");
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Cancel", &objMenuContainer)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateStaffFirstName("Update first name", ptrSalariedStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateStaffLastName("Update second name", ptrSalariedStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateStaffJobRole("Update Job Title", ptrSalariedStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateStaffDepartment("Update Department", ptrSalariedStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateSalariedSalary("Update Salary", ptrSalariedStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateSalariedSeniorStatus("Update Senior status", ptrSalariedStaff)));

		while (!objMenuContainer.GetExitMenu()) {
			system("cls");
			std::cout << "\nUpdating: " << ptrSalariedStaff->GetFullName() << "\n";
			objMenuContainer.Execute();
		}
	} while (!boolExitWhile);
}

void SubMenuUpdateContractStaff::Execute() {
	bool boolExitWhile = false;
	do {
		system("cls");
		std::cout << "Current contract staff\n\n";


		if (_ptrVecContractStaff->size() < 1) {
			std::cout << "There are not currently any contract staff in the system. To update staff details, you first must add at least one staff member\n";
			boolExitWhile = true;
			util::Pause();
			continue;
		}

		util::OutputContractStaffHeader();
		util::OutputContractStaff(_ptrVecContractStaff);

		std::cout << "\nPlease enter the full name of the staff member you would like to update.\nNote: Enter 0 to exit this update screen and return.\n";
		std::cout << "Full name: ";
		std::string strFullName = InputValidator::ValidateString();

		if (strFullName == "0") {
			boolExitWhile = true;
			continue;
		}

		ContractStaff* ptrContractStaff = _ptrStaffManager->GetContractStaff(strFullName);

		while (ptrContractStaff == NULL) {
			std::cout << "No staff member with this name found, please try again (enter 0 to cancel).\n";
			std::cout << "Full name: ";
			strFullName = InputValidator::ValidateString();
			if (strFullName == "0") {
				boolExitWhile = true;
				ptrContractStaff = new ContractStaff();
				continue;
			}
			ptrContractStaff = _ptrStaffManager->GetContractStaff(strFullName);
		}

		MenuContainer objMenuContainer = MenuContainer("Choose action.\n");
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Cancel", &objMenuContainer)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateStaffFirstName("Update first name", ptrContractStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateStaffLastName("Update second name", ptrContractStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateStaffJobRole("Update Job Title", ptrContractStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateStaffDepartment("Update Department", ptrContractStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateContractWage("Update Wage", ptrContractStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateContractHours("Update contracted hours", ptrContractStaff)));
		objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new ActionMenuUpdateContractWeeks("Update contracted weeks", ptrContractStaff)));

		while (!objMenuContainer.GetExitMenu()) {
			system("cls");
			std::cout << "\nUpdating: " << ptrContractStaff->GetFullName() << "\n";
			objMenuContainer.Execute();
		}
	} while (!boolExitWhile);
}

void ActionMenuUpdateStaffFirstName::Execute() {
	std::cout << "\nEnter new first name: ";
	std::string strFirstName = InputValidator::ValidateString(15);

	_ptrStaff->SetFirstName(strFirstName);
	std::cout << "First name updated\n";
	util::Pause();
}

void ActionMenuUpdateStaffLastName::Execute() {
	std::cout << "\nEnter new last name: ";
	std::string strLastName = InputValidator::ValidateString(15);

	_ptrStaff->SetLastName(strLastName);
	std::cout << "Last name updated\n";
	util::Pause();
}

void ActionMenuUpdateStaffJobRole::Execute() {
	std::cout << "\nEnter new job role: ";
	std::string strJobRole = InputValidator::ValidateString(20);

	_ptrStaff->SetJobRole(strJobRole);
	std::cout << "Job role updated\n";
	util::Pause();
}

void ActionMenuUpdateStaffDepartment::Execute() {
	std::cout << "\nEnter new department: ";
	std::string strDepartment = InputValidator::ValidateString(25);

	_ptrStaff->SetDepartment(strDepartment);
	std::cout << "Department updated\n";
	util::Pause();
}

void ActionMenuUpdateSalariedSalary::Execute() {
	std::cout << "\nEnter new salary: ";
	double dSalary = InputValidator::ValidateDouble();

	_ptrSalariedStaff->SetSalary(dSalary);
	std::cout << "Salary updated\n";
	util::Pause();
}

void ActionMenuUpdateSalariedSeniorStatus::Execute() {
	std::cout << "\nEnter new senior status (true or false): ";
	std::string strSeniorStatus = InputValidator::ValidateString(5);

	bool boolStatusIsTrue = util::find_substring_case_insensitive(strSeniorStatus, std::string("true"));

	if (boolStatusIsTrue)
	{
		_ptrSalariedStaff->SetSenior(true);
		std::cout << "Senior status updated\n";
		util::Pause();
	}
	else {
		bool boolStatusIsFalse = util::find_substring_case_insensitive(strSeniorStatus, std::string("false"));

		if (boolStatusIsFalse) {
			_ptrSalariedStaff->SetSenior(false);
			std::cout << "Senior status updated\n";
			util::Pause();
		}
		else {
			std::cout << strSeniorStatus << " is not a valid senior status\n";
			util::Pause();
		}
	}
}

void ActionMenuUpdateContractWage::Execute() {
	std::cout << "\nEnter new wage: ";
	double dWage = InputValidator::ValidateDouble();

	_ptrContractStaff->SetWage(dWage);
	std::cout << "Wage updated\n";
	util::Pause();
}

void ActionMenuUpdateContractHours::Execute() {
	std::cout << "\nEnter new weekly hours (max 20): ";
	double dWeeklyHours = InputValidator::ValidateDouble(20);

	_ptrContractStaff->SetWeeklyHours(dWeeklyHours);
	std::cout << "Weekly hours updated\n";
	util::Pause();
}

void ActionMenuUpdateContractWeeks::Execute() {
	std::cout << "\nEnter new contracted weeks (max 30): ";
	int dContractedWeeks = InputValidator::ValidateInt(30);

	_ptrContractStaff->SetWeeks(dContractedWeeks);
	std::cout << "Contracted weeks updated\n";
	util::Pause();
}

SubMenuViewPayrollBudgetReport::SubMenuViewPayrollBudgetReport(std::string output, BudgetCalculator* budgetCalculator) : MenuCalculateBudgetBase(output, budgetCalculator)
{
	_ptrSalariedStaff = _ptrStaffManager->GetPtrSalariedStaff();
	_ptrContractStaff = _ptrStaffManager->GetPtrContractStaff();
}

void SubMenuViewPayrollBudgetReport::Execute() {
	MenuContainer objMenuContainer = MenuContainer("Choose action.\n");
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuExit("Return", &objMenuContainer)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuSaveLoad("Save this report", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new SubMenuAddStaffMember("Add staff member", _ptrStaffManager)));
	objMenuContainer.AddMenuItem(std::unique_ptr<MenuItem>(new MenuUpdateStaff("Update staff member", _ptrStaffManager)));

	while (!objMenuContainer.GetExitMenu()) {
		system("cls");
		struct std::tm timeNow = util::GetCurrentDateTimeStruct();

		std::cout << "Payroll budget report\n\n";

		if (_ptrSalariedStaff->size() > 0 || _ptrContractStaff->size() > 0) {
			std::cout.imbue(std::locale("en_GB"));
			std::cout << "Report date: " << std::put_time(&timeNow, "%c") << "\n\n";

			if (_ptrStaffManager->CountSeniorStaff() > 0) {
				std::cout << "Number of senior staff assigned to project: " << _ptrStaffManager->CountSeniorStaff() << "\n";
				std::cout << "Names: ";
				std::for_each(_ptrSalariedStaff->begin(), _ptrSalariedStaff->end(), [](SalariedStaff salariedStaff) {
					if (salariedStaff.GetSenior()) std::cout << salariedStaff.GetFullName() << ", ";
					});
				std::cout << "\b\b" << "  " << "\n\n";
			}
			else {
				std::cout << "No senior staff added for this project.\n\n";
			}

			if (_ptrStaffManager->CountStandardStaff() > 0) {
				std::cout << "Number of salaried staff assigned to project: " << _ptrStaffManager->CountStandardStaff() << "\n";
				std::cout << "Names: ";
				std::for_each(_ptrSalariedStaff->begin(), _ptrSalariedStaff->end(), [](SalariedStaff salariedStaff) {
					if (!salariedStaff.GetSenior()) std::cout << salariedStaff.GetFullName() << ", ";
					});
				std::cout << "\b\b" << "  " << "\n\n";
			}
			else {
				std::cout << "No salaried staff added for this project.\n\n";
			}

			if (_ptrStaffManager->CountContractStaff() > 0) {
				std::cout << "Number of contract staff assigned to project: " << _ptrStaffManager->CountContractStaff() << "\n";
				std::cout << "Names: ";
				std::for_each(_ptrContractStaff->begin(), _ptrContractStaff->end(), [](Staff contractStaff) {
					std::cout << contractStaff.GetFullName() << ", ";
					});
				std::cout << "\b\b" << "  " << "\n\n";
			}
			else {
				std::cout << "No contract staff added for this project.\n\n";
			}

			_ptrBudgetCalculator->Calculate();

			std::cout.precision(2);
			std::cout << std::fixed << "--- Costs per year ---\n\n";
			std::cout << "Senior staff total salary: " << char(156) << _ptrBudgetCalculator->GetSeniorSalaryTotal() << "\n";
			std::cout << "Senior staff average salary: " << char(156) << _ptrBudgetCalculator->GetSeniorSalaryAverage() << "\n\n";
			std::cout << "Salaried staff total salary: " << char(156) << _ptrBudgetCalculator->GetSalariedSalaryTotal() << "\n";
			std::cout << "Salaried staff average salary: " << char(156) << _ptrBudgetCalculator->GetSalariedSalaryAverage() << "\n\n";
			std::cout << "Contractor total costs: " << char(156) << _ptrBudgetCalculator->GetContractorCostTotal() << "\n";
			std::cout << "Contractor average costs: " << char(156) << _ptrBudgetCalculator->GetContractorCostAverage() << "\n\n";
			std::cout << "Total payroll cost: " << char(156) << _ptrBudgetCalculator->GetTotalPayroll() << "\n";
			std::cout << "Total payroll cost (with overrun): (Min - " << _ptrBudgetCalculator->GetMinOverPercent() << "%) " << char(156) << _ptrBudgetCalculator->GetMinimumOverBudget() << " - "
				<< "(Max - " << _ptrBudgetCalculator->GetMaxOverPercent() << "%) " << char(156) << _ptrBudgetCalculator->GetMaximumOverBudget() << "\n\n";

			if (!_ptrBudgetCalculator->GetProjectIsDefaultDuration()) {
				std::cout << "--- Costs over tatal project length of " << _ptrBudgetCalculator->GetProjectLength() << " years " << "---\n\n";
				std::cout << "Senior staff total salary: " << char(156) << _ptrBudgetCalculator->GetProjLenSeniorSalaryTotal() << "\n";
				std::cout << "Salaried staff total salary: " << char(156) << _ptrBudgetCalculator->GetProjLenSalariedSalaryTotal() << "\n";
				std::cout << "Contractor total costs: " << char(156) << _ptrBudgetCalculator->GetProjLenContractPayTotal() << "\n";
				std::cout << "Total payroll cost: " << char(156) << _ptrBudgetCalculator->GetProjLenTotalPayroll() << "\n";
				std::cout << "Total payroll cost (with overrun): (Min - " << _ptrBudgetCalculator->GetMinOverPercent() << "%) " << char(156) << _ptrBudgetCalculator->GetProjLenMinimumOverBudget() << " - "
					<< "(Max - " << _ptrBudgetCalculator->GetMaxOverPercent() << "%) " << char(156) << _ptrBudgetCalculator->GetProjLenMaximumOverBudget() << "\n\n";
			}

			objMenuContainer.Execute();
		}
		else {
			std::cout << "ERROR: You cannot calculate a payroll budget with no staff in the system.\nPlease go and add at least one staff member, or the recommended 3 senior, 5 salaried and 5 contract staff\n";
			objMenuContainer.SetExitMenu(true);
			util::Pause();
		}
		
	}
}

void SubMenuUpdateMinOverrun::Execute() {
	system("cls");
	std::cout << "Update minimum budget overrun percentage (0-100, must be less than current maximum budget overrun)\n";
	std::cout << "Enter new minimum budget overun percentage: ";
	double dInput = InputValidator::ValidateDouble(0, 100);
	if (dInput < _ptrBudgetCalculator->GetMaxOverPercent()) {
		_ptrBudgetCalculator->SetMinOverPercent(dInput);
		std::cout << "Minimum budget overrun updated to "  << _ptrBudgetCalculator->GetMinOverPercent() << "%\n";
		util::Pause();
	}
	else {
		std::cout << "Error: Minimum budget overun could not be set, is more than the current max budget overrun\n";
		util::Pause();
	}
}

void SubMenuUpdateMaxOverrun::Execute() {
	system("cls");
	std::cout << "Update maxiumum budget overrun percentage (0-100, must be more than current minimum budget overrun)\n";
	std::cout << "Enter new maximum budget overun percentage: ";
	double dInput = InputValidator::ValidateDouble(0, 100);
	if (dInput > _ptrBudgetCalculator->GetMinOverPercent()) {
		_ptrBudgetCalculator->SetMaxOverPercent(dInput);
		std::cout << "Maximum budget overrun updated to "  << _ptrBudgetCalculator->GetMaxOverPercent() << "%\n";
		util::Pause();
	}
	else {
		std::cout << "Error: Maximum budget overun could not be set, is less than the current minimum budget overrun\n";
		util::Pause();
	}
}

void SubMenuUpdateProjectLength::Execute() {
	system("cls");
	std::cout << "Update project length (in years, e.g. 1 for one year, 1.5 for one year and a half)\n";
	std::cout << "Enter project length: ";
	double dInput = InputValidator::ValidateDouble();
	_ptrBudgetCalculator->SetProjectLength(dInput);
	std::cout << "Project length updated to "  << _ptrBudgetCalculator->GetProjectLength() << " years\n";
	util::Pause();
}