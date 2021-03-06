#include "interface.h"

inline void flush() {
	while (getchar() != '\n');
}

void ManagerInterface::login()
{
	Authority auth;
	std::string id, password;
	while (true)
	{
		cout << "---Input your id and password ---" << endl;
		cout << "if you wanna out, press '.' and press enter " << endl;
		//cout << "if you want to go index mode, type 'index' and enter " << endl;
		
		cout << "id : ";
		cin >> id;
		if (!id.compare("."))
		{
			end = -1;
			return;
		}
		/*
		if (!id.compare("index"))
		{
			nextState(&ManagerInterface::index_menu);
			return;
		}
		*/
		cout << "password : ";
		cin >> password;

		auth = pIMM->verify(id, password);
		switch(auth)
		{
		case auth_noneid: cout << "check your id" << endl; break;
		case auth_nonepw: cout << "check your password" << endl; break;
		case auth_admin: cout << "Go to admin page" << endl; nextState(&ManagerInterface::admin_menu); return;
		case auth_normal: cout << "Go to normal page" << endl; nextState(&ManagerInterface::normal_menu); return;
		}
	}
}

void ManagerInterface::admin_menu()
{
	RM_errcode err;

	while (true)
	{
		int menu;
		cout << "---Welcome to admin Menu---" << endl;
		cout << "1. Member View" << endl;
		cout << "2. Member Insert" << endl;
		cout << "3. Member Update" << endl;
		cout << "4. Member Remove" << endl;
		cout << "5. Member Search" << endl;
		cout << "6. Lecture View" << endl;
		cout << "7. Lecture Insert" << endl;
		cout << "8. Lecture Update" << endl;
		cout << "9. Lecture Remove" << endl;
		cout << "10. Lecture Search" << endl;
		cout << "11. Purchase View" << endl;
		cout << "12. Purchase Insert" << endl;
		cout << "13. Purchase Update" << endl;
		cout << "14. Purchase Remove" << endl;
		cout << "15. Purchase Search" << endl;
		cout << "0. end" << endl;
		cout << "-----------------------" << endl;

		cin >> menu;
		flush();

		switch (menu)
		{
		case 0: end = true; return;
		case 1: nextState(&ManagerInterface::member_retrieve); return;
		case 2: nextState(&ManagerInterface::member_insert); return;
		case 3: nextState(&ManagerInterface::member_update); return;
		case 4: nextState(&ManagerInterface::member_remove); return;
		case 5: nextState(&ManagerInterface::member_search); return;
		case 6: nextState(&ManagerInterface::lecture_retrieve); return;
		case 7: nextState(&ManagerInterface::lecture_insert); return;
		case 8: nextState(&ManagerInterface::lecture_update); return;
		case 9: nextState(&ManagerInterface::lecture_remove); return;
		case 10: nextState(&ManagerInterface::lecture_search); return;
		case 11: nextState(&ManagerInterface::purchase_retrieve); return;
		case 12: nextState(&ManagerInterface::purchase_insert); return;
		case 13: nextState(&ManagerInterface::purchase_update); return;
		case 14: nextState(&ManagerInterface::purchase_remove); return;
		case 15: nextState(&ManagerInterface::purchase_search); return;
		default: nextState(&ManagerInterface::admin_menu); return;
		}
	}
}
void ManagerInterface::normal_menu()
{
	RM_errcode err;

	while (true)
	{
		int menu;
		cout << "---Welcome to normal Menu---" << endl;
		cout << "1. Member View" << endl;
		cout << "2. Member Update" << endl;
		cout << "3. Member Remove" << endl;
		cout << "4. Lecture Retrieve" << endl;
		cout << "5. Lecture Search" << endl;
		cout << "6. Purchase View" << endl;
		cout << "7. Purchase Insert" << endl;
		cout << "8. Purchase Update" << endl;
		cout << "9. Purchase Remove" << endl;
		cout << "10. Purchase Search" << endl;
		cout << "0. end" << endl;
		cout << "-----------------------" << endl;

		cin >> menu;
		flush();

		switch (menu)
		{
		case 0: end = true; return;
		case 1: nextState(&ManagerInterface::member_my_retrieve); return;
		case 2: nextState(&ManagerInterface::member_my_update); return;
		case 3: nextState(&ManagerInterface::member_my_remove); return;
		case 4: nextState(&ManagerInterface::lecture_my_retrieve); return;
		case 5: nextState(&ManagerInterface::lecture_my_search); return;
		case 6: nextState(&ManagerInterface::purchase_my_retrieve); return;
		case 7: nextState(&ManagerInterface::purchase_my_insert); return;
		case 8: nextState(&ManagerInterface::purchase_my_update); return;
		case 9: nextState(&ManagerInterface::purchase_my_remove); return;
		case 10: nextState(&ManagerInterface::purchase_my_search); return;
		default: nextState(&ManagerInterface::normal_menu); return;
		}
	}
}

void ManagerInterface::member_retrieve()
{
	std::vector<Member> list;
	cout << "----------------------- Member retrieve -------------------------" << endl;
	pIMM->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Member retrieve -------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();

}
void ManagerInterface::member_insert()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Member insert -------------------------" << endl;
	cout << "if you want to cancel typing member data, press '.' and enter" << endl;
	cout << "---------------------------------------------------------------" << endl;
	
	cout << "Enter the ID : ";
	cin >> element;
	if (!element.compare(".")) goto MEMBER_INSERT_OUT;
	m.update_ID(element);

	cout << "Enter the password : ";
	cin >> element;
	if (!element.compare(".")) goto MEMBER_INSERT_OUT;
	m.update_Password(element);

	cout << "Enter the level : ";
	cin >> element;
	if (!element.compare(".")) goto MEMBER_INSERT_OUT;
	m.update_Level(element[0]);

	cout << "Enter the Name : ";
	cin >> element;
	if (!element.compare(".")) goto MEMBER_INSERT_OUT;
	m.update_Name(element);

	cout << "Enter the address : ";
	cin >> element;
	if (!element.compare(".")) goto MEMBER_INSERT_OUT;
	m.update_Address(element);

	cout << "Enter the Phone Number : ";
	cin >> element;
	if (!element.compare(".")) goto MEMBER_INSERT_OUT;
	m.update_PhoneNumber(element);

	cout << "Enter the Mileage: ";
	cin >> element;
	if (!element.compare(".")) goto MEMBER_INSERT_OUT;
	m.update_mileage(element.c_str());

	err = pIMM->insert(m);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	MEMBER_INSERT_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Member insert -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::member_update()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Member update -------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	cout << m;
	cout << "---------------------------------------------------------------" << endl;

	cout << "If you want to update, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto MEMBER_UPDATE_OUT;
	}

	cout << "If you do not want to change, Press '.' and Enter!" << endl;
	cout << "Enter the password : ";
	cin >> element;
	if(element.compare("."))
		m.update_Password(element);

	cout << "Enter the level : ";
	cin >> element;
	if (element.compare("."))
		m.update_Level(element[0]);

	cout << "Enter the Name : ";
	cin >> element;
	if (element.compare("."))
		m.update_Name(element);

	cout << "Enter the address : ";
	cin >> element;
	if (element.compare("."))
		m.update_Address(element);

	cout << "Enter the Phone Number : ";
	cin >> element;
	if (element.compare("."))
		m.update_PhoneNumber(element);

	cout << "Enter the Mileage: ";
	cin >> element;
	if (element.compare("."))
		m.update_mileage(element.c_str());

	err = pIMM->update(m);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

MEMBER_UPDATE_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Member update -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::member_remove()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Member remove-------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	cout << m;
	cout << "---------------------------------------------------------------" << endl;

	cout << "If you want to remove, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto MEMBER_REMOVE_OUT;
	}

	err = pIMM->remove(m);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_noauth)
		{
			cout << "You delete yourself\n" << endl;
			end = true;
			return;
		}
		cout << "Fail!" << endl;
	}

	MEMBER_REMOVE_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Member remove -------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::member_search()
{
	RM_errcode err;
	std::string element;
	Member source;

	cout << "----------------------- Member search -------------------------" << endl;
	cout << "If you want to search, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto MEMBER_SEARCH_OUT;
	}

	cout << "Enter the ID : ";
	cin >> element;
	source.update_ID(element);
	err = pIMM->search(source, m);

	cout << "<<Current Member>>" << endl;
	cout << m;
	cout << "---------------------------------------------------------------" << endl;

	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

MEMBER_SEARCH_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Member search -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}

void ManagerInterface::lecture_retrieve()
{
	std::vector<Lecture> list;
	cout << "----------------------- Lecture retrieve -------------------------" << endl;
	pILM->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Lecture retrieve -------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::lecture_insert()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Lecture insert -------------------------" << endl;
	cout << "if you want to cancel typing member data, press '.' and enter" << endl;
	cout << "---------------------------------------------------------------" << endl;

	cout << "Enter the ID : ";
	cin >> element;
	if (!element.compare(".")) goto LECTURE_INSERT_OUT;
	l.update_lectureid(element);

	cout << "Enter the Subject : ";
	cin >> element;
	if (!element.compare(".")) goto LECTURE_INSERT_OUT;
	l.update_subject(element);

	cout << "Enter the Level : ";
	cin >> element;
	if (!element.compare(".")) goto LECTURE_INSERT_OUT;
	l.update_level(element);

	cout << "Enter the Price : ";
	cin >> element;
	if (!element.compare(".")) goto LECTURE_INSERT_OUT;
	l.update_price(element);

	cout << "Enter the Extension : ";
	cin >> element;
	if (!element.compare(".")) goto LECTURE_INSERT_OUT;
	l.update_extension(element);

	cout << "Enter the DueDate : ";
	cin >> element;
	if (!element.compare(".")) goto LECTURE_INSERT_OUT;
	l.update_duedate(element);

	cout << "Enter the Name of teacher : ";
	cin >> element;
	if (!element.compare(".")) goto LECTURE_INSERT_OUT;
	l.update_nameofteacher(element);

	cout << "Enter the Textbook : ";
	cin >> element;
	if (!element.compare(".")) goto LECTURE_INSERT_OUT;
	l.update_textbook(element);

	err = pILM->insert(l);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	LECTURE_INSERT_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Lecture insert -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::lecture_update()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Lecture update -------------------------" << endl;
	cout << "<<Current Lecture>>" << endl;
	cout << l;
	cout << "---------------------------------------------------------------" << endl;

	cout << "If you want to update, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto LECTURE_UPDATE_OUT;
	}

	cout << "If you do not want to change, Press '.' and Enter!" << endl;
	
	
	cout << "Enter the Subject : ";
	cin >> element;
	if (element.compare("."))
		l.update_subject(element);

	cout << "Enter the Level : ";
	cin >> element;
	if (element.compare("."))
		l.update_level(element);

	cout << "Enter the Price : ";
	cin >> element;
	if (element.compare("."))
		l.update_price(element);

	cout << "Enter the Extension : ";
	cin >> element;
	if (element.compare("."))
		l.update_extension(element);

	cout << "Enter the DueDate : ";
	cin >> element;
	if (element.compare("."))
		l.update_duedate(element);

	cout << "Enter the Name of teacher : ";
	cin >> element;
	if (element.compare("."))
		l.update_nameofteacher(element);

	cout << "Enter the Textbook : ";
	cin >> element;
	if (element.compare("."))
		l.update_textbook(element);

	err = pILM->update(l);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	LECTURE_UPDATE_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "---Lecture update---" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::lecture_remove()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Lecture remove-------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	cout << l;
	cout << "---------------------------------------------------------------" << endl;

	cout << "If you want to remove, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto LECTURE_REMOVE_OUT;
	}

	err = pILM->remove(l);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_noauth)
		{
			cout << "You delete yourself\n" << endl;
			end = true;
			return;
		}
		cout << "Fail!" << endl;
	}

	LECTURE_REMOVE_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Lecture remove-------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::lecture_search()
{
	RM_errcode err;
	std::string element;
	Lecture source;
	cout << "----------------------- Lecture search-------------------------" << endl;

	cout << "If you want to search, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;

	char dat = getchar();
	if (dat == '.')
	{
		goto LECTURE_SEARCH_OUT;
	}

	cout << "Enter the ID : ";
	cin >> element;
	source.update_lectureid(element);
	err = pILM->search(source, l);

	cout << "<<Current Lecture>>" << endl;
	cout << l;
	cout << "---------------------------------------------------------------" << endl;

	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	LECTURE_SEARCH_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "---Lecture search---" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}

void ManagerInterface::purchase_retrieve()
{
	std::vector<Purchase> list;
	cout << "----------------------- Purchase retrieve -------------------------" << endl;
	pPM->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Purchase retrieve -------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_insert()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Purchase insert -------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	cout << m;
	cout << "<<Current Lecture>>" << endl;
	cout << l;
	cout << "---------------------------------------------------------------" << endl;
	cout << "Purchase Element is made by above member and lecture" << endl;
	cout << "If you want to change it, search Member or search Lecture first" << endl;
	cout << "if you want to cancel typing member data, press '.' and enter" << endl;

	p.update_purchaseid("");
	p.update_lectureid(l.getKey(0));
	p.update_memberid(m.getKey(0));
	
	cout << "Enter the Mileage : ";
	cin >> element;
	if (!element.compare(".")) goto PURCHASE_INSERT_OUT;
	p.update_mileage(element.c_str());

	if (pIMM->search(m, m) != RM_valid)
	{
		cout << "There is no member in member list!\n" << endl;
		goto PURCHASE_INSERT_OUT;
	}
	if (pILM->search(l, l) != RM_valid)
	{
		cout << "There is no member in member list!\n" << endl;
		goto PURCHASE_INSERT_OUT;
	}

	err = pPM->insert(p);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_redundant)
			cout << "Redundant Record!" << endl;
		else
			cout << "Fail!" << endl;
	}

	PURCHASE_INSERT_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Purchase insert-------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_update()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Purchase update -------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	cout << m;
	cout << "<<Current Lecture>>" << endl;
	cout << l;
	cout << "<<Current Purchase>>" << endl;
	cout << p;
	cout << "---------------------------------------------------------------" << endl;
	cout << "Purchase Element is made by above member and lecture" << endl;
	cout << "If you want to change it, search Member or search Lecture first" << endl;
	cout << "If you want to update, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;

	char dat = getchar();
	if (dat == '.')
	{
		goto PURCHASE_UPDATE_OUT;
	}

	p.update_lectureid(l.getKey(0));
	p.update_memberid(m.getKey(0));

	cout << "If you do not want to change, Press '.' and Enter!" << endl;
	cout << "Enter the Mileage : ";
	cin >> element;
	if(element.compare("."))
		p.update_mileage(element.c_str());

	err = pPM->update(p);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_redundant)
			cout << "Redundant Record!" << endl;
		else
			cout << "Fail!" << endl;
	}

PURCHASE_UPDATE_OUT:

	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Purchase update -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_remove()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Purchase remove -------------------------" << endl;
	cout << "<<Current Purchase>>" << endl;
	cout << p;
	cout << "---------------------------------------------------------------" << endl;

	cout << "If you want to remove, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto PURCHASE_REMOVE_OUT;
	}

	err = pPM->remove(p);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_noauth)
		{
			cout << "You delete yourself\n" << endl;
			end = true;
			return;
		}
		cout << "Fail!" << endl;
	}

	PURCHASE_REMOVE_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Purchase remove -------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_search()
{
	RM_errcode err;
	std::string element;
	Purchase source;
	cout << "----------------------- Purchase search -------------------------" << endl;

	cout << "If you want to update, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;

	char dat = getchar();
	if (dat == '.')
	{
		goto PURCHASE_SEARCH_OUT;
	}

	cout << "Enter the ID : ";
	cin >> element;
	source.update_purchaseid(element);
	err = pPM->search(source, p);

	cout << "<<Current Purchase>>" << endl;
	cout << p;
	cout << "---------------------------------------------------------------" << endl;

	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	PURCHASE_SEARCH_OUT:
	nextState(&ManagerInterface::admin_menu);
	cout << "----------------------- Purchase search -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}

void ManagerInterface::member_my_retrieve()
{
	cout << "----------------------- Member my_retrieve -------------------------" << endl;
	pIMM->my_retrieve(m);
	
	cout << m << endl;

	nextState(&ManagerInterface::normal_menu);
	cout << "---------------------------------------------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::member_my_update()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Member my_update -------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	pIMM->my_retrieve(m);
	cout << m;
	cout << "---------------------------------------------------------------" << endl;

	cout << "If you want to update, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto MEMBER_MY_UPDATE_OUT;
	}

	cout << "If you do not wanna change, Press '.' and Enter!" << endl;
	cout << "Enter the password : ";
	cin >> element;
	if (element.compare("."))
		m.update_Password(element);

	cout << "Enter the level : ";
	cin >> element;
	if (element.compare("."))
		m.update_Level(element[0]);

	cout << "Enter the Name : ";
	cin >> element;
	if (element.compare("."))
		m.update_Name(element);

	cout << "Enter the address : ";
	cin >> element;
	if (element.compare("."))
		m.update_Address(element);

	cout << "Enter the Phone Number : ";
	cin >> element;
	if (element.compare("."))
		m.update_PhoneNumber(element);

	cout << "Enter the Mileage: ";
	cin >> element;
	if (element.compare("."))
		m.update_mileage(element.c_str());

	err = pIMM->my_update(m);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

MEMBER_MY_UPDATE_OUT:
	nextState(&ManagerInterface::normal_menu);
	cout << "---Member my_update---" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::member_my_remove()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Member my_remove -------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	pIMM->my_retrieve(m);
	cout << m;
	cout << "---------------------------------------------------------------" << endl;

	cout << "If you want to remove, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto MEMBER_MY_REMOVE_OUT;
	}

	err = pIMM->my_remove();
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_noauth)
		{
			cout << "You delete yourself\n" << endl;
			end = true;
			return;
		}
		cout << "Fail!" << endl;
	}

	MEMBER_MY_REMOVE_OUT:
	nextState(&ManagerInterface::normal_menu);
	cout << "---Member update---" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::lecture_my_retrieve()
{
	std::vector<Lecture> list;
	cout << "----------------------- Lecture my_retrieve -------------------------" << endl;
	pILM->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	nextState(&ManagerInterface::normal_menu);
	cout << "---------------------------------------------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::lecture_my_search()
{
	RM_errcode err;
	std::string element;
	Lecture source;
	cout << "----------------------- Lecture my_search -------------------------" << endl;

	cout << "If you want to search, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto LECTURE_MY_RETRIEVE_OUT;
	}

	cout << "Enter the ID : ";
	cin >> element;
	source.update_lectureid(element);
	err = pILM->search(source, l);

	cout << "<<Current Lecture>>" << endl;
	cout << l;
	cout << "---------------------------------------------------------------" << endl;

	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	LECTURE_MY_RETRIEVE_OUT:
	nextState(&ManagerInterface::normal_menu);
	cout << "----------------------- Lecture my_search -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_my_retrieve()
{
	std::vector<Purchase> list;
	Member m;
	cout << "----------------------- Purchase my_retrieve -------------------------" << endl;
	pIMM->my_retrieve(m);
	pPM->my_retrieve(m, list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	nextState(&ManagerInterface::normal_menu);
	cout << "----------------------- Purchase my_retrieve -------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_my_insert()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Purchase my_insert -------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	pIMM->my_retrieve(m);
	cout << m;
	cout << "<<Current Lecture>>" << endl;
	cout << l;
	cout << "Purchase Element is made by above member and lecture" << endl;
	cout << "If you wanna change it, search Member or search Lecture first" << endl;
	cout << "If you want to update, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;

	char dat = getchar();
	if (dat == '.')
	{
		goto MEMBER_MY_INSERT_OUT;
	}

	if (pIMM->search(m, m) != RM_valid)
	{
		cout << "There is no member in member list!\n" << endl;
		goto MEMBER_MY_INSERT_OUT;
	}
	if (pILM->search(l, l) != RM_valid)
	{
		cout << "There is no member in member list!\n" << endl;
		goto MEMBER_MY_INSERT_OUT;
	}

	p.update_purchaseid("");
	p.update_lectureid(l.getKey(0));
	p.update_memberid(m.getKey(0));

	cout << "Enter the Mileage : ";
	cin >> element;
	p.update_mileage(element.c_str());

	err = pPM->my_insert(m, p);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_redundant)
			cout << "Redundant Record!" << endl;
		else
			cout << "Fail!" << endl;
	}

	MEMBER_MY_INSERT_OUT:
	nextState(&ManagerInterface::normal_menu);
	cout << "----------------------- Purchase my_insert -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_my_update()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Purchase my_update -------------------------" << endl;
	cout << "<<Current Member>>" << endl;
	pIMM->my_retrieve(m);
	cout << m;
	cout << "<<Current Lecture>>" << endl;
	cout << l;
	cout << "<<Current Purchase>>" << endl;
	cout << p;
	cout << "Purchase Element is made by above member and lecture" << endl;
	cout << "If you wanna change it, search Member or search Lecture first" << endl;
	cout << "If you want to insert, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;

	char dat = getchar();
	if (dat == '.')
	{
		goto PURCHASE_MY_UPDATE_OUT;
	}

	p.update_lectureid(l.getKey(0));
	p.update_memberid(m.getKey(0));

	cout << "Enter the Mileage : ";
	cin >> element;
	p.update_mileage(element.c_str());

	err = pPM->my_update(m, p);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_redundant)
			cout << "Redundant Record!" << endl;
		else
			cout << "Fail!" << endl;
	}

	PURCHASE_MY_UPDATE_OUT:
	nextState(&ManagerInterface::normal_menu);
	cout << "----------------------- Purchase my_update -------------------------" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_my_remove()
{
	RM_errcode err;
	std::string element;
	cout << "----------------------- Purchase my_remove -------------------------" << endl;
	cout << "<<Current Purchase>>" << endl;
	cout << p;
	cout << "---------------------------------------------------------------" << endl;

	cout << "If you want to update, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '.')
	{
		goto PURCHASE_MY_REMOVE_OUT;
	}
	pIMM->my_retrieve(m);
	err = pPM->my_remove(m, p);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_noauth)
		{
			cout << "You delete yourself\n" << endl;
			end = true;
			return;
		}
		cout << "Fail!" << endl;
	}

	PURCHASE_MY_REMOVE_OUT:
	nextState(&ManagerInterface::normal_menu);
	cout << "----------------------- Purchase my_remove -------------------------" << endl;
	cout << "Press the Button" << endl;
	getchar();
}
void ManagerInterface::purchase_my_search()
{
	RM_errcode err;
	std::string element;
	Purchase source;
	cout << "----------------------- Purchase my_search -------------------------" << endl;

	cout << "<<Current Member>>" << endl;
	pIMM->my_retrieve(m);
	cout << m;
	cout << "<<Current Lecture>>" << endl;
	cout << l;

	cout << "We find the purchase  element using above member and lecture" << endl;
	cout << "If you want to search, press any button" << endl;
	cout << "if you want to quit, press '.' button" << endl;
	char dat = getchar();
	if (dat == '0')
	{
		goto PURCHASE_MY_SERACH_OUT;
	}

	pIMM->my_retrieve(m);
	err = pPM->my_search(m, l, p);

	cout << "<<Current Purchase>>" << endl;
	cout << p;
	cout << "---------------------------------------------------------------" << endl;

	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	PURCHASE_MY_SERACH_OUT:
	nextState(&ManagerInterface::normal_menu);
	cout << "---Purchase search---" << endl;
	flush();
	cout << "Press the Button" << endl;
	getchar();
}

/***************************************************************************/

void ManagerInterface::index_menu()
{
	while (true)
	{
		int menu;
		cout << "---Welcome to Index Menu---" << endl;
		cout << "1. Member View" << endl;
		cout << "2. Member Insert" << endl;
		cout << "3. Member Update" << endl;
		cout << "4. Member Remove" << endl;
		cout << "5. Member Search" << endl;
		cout << "0. end" << endl;
		cout << "-----------------------" << endl;

		cin >> menu;

		switch (menu)
		{
		case 0: end = true; return;
		case 1: nextState(&ManagerInterface::memberindex_retrieve); return;
		case 2: nextState(&ManagerInterface::memberindex_insert); return;
		case 3: nextState(&ManagerInterface::memberindex_update); return;
		case 4: nextState(&ManagerInterface::memberindex_remove); return;
		case 5: nextState(&ManagerInterface::memberindex_search); return;
		}
	}
}

void ManagerInterface::memberindex_retrieve()
{
	std::vector<Member> list;
	cout << "---Member retrieve---" << endl;
	pIMM->retrieve(list);
	for (int i = 0; i < list.size(); ++i)
		cout << list[i] << endl;

	nextState(&ManagerInterface::index_menu);
	cout << "---Member retrieve---" << endl;
	cout << "Press the Button" << endl;
	getchar();
	getchar();
}
void ManagerInterface::memberindex_insert()
{
	RM_errcode err;
	std::string element;
	cout << "---Member insert---" << endl;

	cout << "Enter the ID : ";
	cin >> element;
	m.update_ID(element);

	cout << "Enter the password : ";
	cin >> element;
	m.update_Password(element);

	cout << "Enter the level : ";
	cin >> element;
	m.update_Level(element[0]);

	cout << "Enter the Name : ";
	cin >> element;
	m.update_Name(element);

	cout << "Enter the address : ";
	cin >> element;
	m.update_Address(element);

	cout << "Enter the Phone Number : ";
	cin >> element;
	m.update_PhoneNumber(element);

	cout << "Enter the Mileage: ";
	cin >> element;
	m.update_mileage(element.c_str());

	err = pIMM->insert(m);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	nextState(&ManagerInterface::index_menu);
	cout << "---Member insert---" << endl;
	cout << "Press the Button" << endl;
	getchar();
	getchar();
}
void ManagerInterface::memberindex_update()
{
	RM_errcode err;
	std::string element;
	cout << "---Member update---" << endl;
	cout << "<<Current Member>>" << endl;
	cout << m;
	cout << "<<Current Member>>" << endl;

	cout << "wanna update?" << endl << " if not, press '0'. or press any else  " << endl;
	char dat;
	cin >> dat;
	if (dat == '0')
	{
		nextState(&ManagerInterface::index_menu);
		cout << "---Member update---" << endl;
		cout << "Press the Button" << endl;
		getchar();
		getchar();
		return;
	}

	cout << "If you do not wanna change, Press '.' and Enter!" << endl;
	cout << "Enter the password : ";
	cin >> element;
	if (element.compare("."))
		m.update_Password(element);

	cout << "Enter the level : ";
	cin >> element;
	if (element.compare("."))
		m.update_Level(element[0]);

	cout << "Enter the Name : ";
	cin >> element;
	if (element.compare("."))
		m.update_Name(element);

	cout << "Enter the address : ";
	cin >> element;
	if (element.compare("."))
		m.update_Address(element);

	cout << "Enter the Phone Number : ";
	cin >> element;
	if (element.compare("."))
		m.update_PhoneNumber(element);

	cout << "Enter the Mileage: ";
	cin >> element;
	if (element.compare("."))
		m.update_mileage(element.c_str());

	err = pIMM->update(m);
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	nextState(&ManagerInterface::index_menu);
	cout << "---Member update---" << endl;
	cout << "Press the Button" << endl;
	getchar();
	getchar();
}
void ManagerInterface::memberindex_remove()
{
	RM_errcode err;
	std::string element;
	cout << "---Member remove---" << endl;
	cout << "<<Current Member>>" << endl;
	cout << m;
	cout << "<<Current Member>>" << endl;

	cout << "wanna remove?" << endl << " if not, press '0'. or press any else  " << endl;
	char dat;
	cin >> dat;
	if (dat == '0')
	{
		nextState(&ManagerInterface::index_menu);
		cout << "---Member remove---" << endl;
		cout << "Press the Button" << endl;
		getchar();
		getchar();
		return;
	}

	err = pIMM->remove(m);
	m = Member();
	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		if (err == RM_noauth)
		{
			cout << "You delete yourself\n" << endl;
			end = true;
			return;
		}
		cout << "Fail!" << endl;
	}

	nextState(&ManagerInterface::index_menu);
	cout << "---Member update---" << endl;
	cout << "Press the Button" << endl;
	getchar();
	getchar();
}
void ManagerInterface::memberindex_search()
{
	RM_errcode err;
	std::string element;
	cout << "---Member search---" << endl;

	cout << "wanna search?" << endl << " if not, press '0'. or press any else  " << endl;
	char dat;
	cin >> dat;
	if (dat == '0')
	{
		nextState(&ManagerInterface::index_menu);
		cout << "---Member search---" << endl;
		cout << "Press the Button" << endl;
		getchar();
		getchar();
		return;
	}

	Member source;
	cout << "Enter the ID : ";
	cin >> element;
	source.update_ID(element);
	err = pIMM->search(source, m);

	cout << "<<Current Member>>" << endl;
	cout << m;
	cout << "<<Current Member>>" << endl;

	if (err == RM_valid)
	{
		cout << "Success!" << endl;
	}
	else
	{
		cout << "Fail!" << endl;
	}

	nextState(&ManagerInterface::index_menu);
	cout << "---Member search---" << endl;
	cout << "Press the Button" << endl;
	getchar();
	getchar();
}


void ManagerInterface::play()
{
	RecordFile <Purchase>* Purchasefile = new RecordFile<Purchase>(DelimFieldBuffer('|', STDMAXBUF));
	pPM = new PurchaseManager("fileOfPurchase.dat", Purchasefile);

	TextIndexedFile <Member>* MemberIndexfile = new TextIndexedFile<Member>(DelimFieldBuffer('|', STDMAXBUF), 16);
	MemberIndexfile->initilaize("fileOfMember");
	pIMM = new MemberIndexManager("fileOfMember", MemberIndexfile);

	TextIndexedFile <Lecture>* LectureIndexfile = new TextIndexedFile<Lecture>(DelimFieldBuffer('|', STDMAXBUF), 16);
	MemberIndexfile->initilaize("fileOfLecture");
	pILM = new LectureIndexManager("fileOfLecture", LectureIndexfile);
	
	pIMM->setPurchaseManager(*pPM);
	pILM->setPurchaseManager(*pPM);

	while (!end)
	{
		(this->*state)();
	}

	delete MemberIndexfile;
	delete LectureIndexfile;
	delete Purchasefile;
	delete pPM;

}