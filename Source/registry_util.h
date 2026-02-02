#pragma once

bool global_registry( bool return_only, bool add, bool as_admin = false );
bool CreateAdminStartupTask(const CString& exePath);
bool DeleteAdminStartupTask();
bool IsLoggedInUserAdmin();
CString GetLoggedInUserName();
