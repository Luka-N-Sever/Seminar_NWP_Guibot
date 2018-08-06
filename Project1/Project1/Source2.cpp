#include "Header.h"
#include "Header1.h"

void inline Remove(HWND wh) {
	int z = 0;
	z = SendMessage(GetDlgItem(wh, IDC_LB), LB_GETCURSEL, NULL, NULL);
	if (z != LB_ERR)
		SendMessage(GetDlgItem(wh, IDC_LB), LB_DELETESTRING, z, NULL);
	z = SendMessage(GetDlgItem(wh, IDC_LB), LB_GETCOUNT, NULL, NULL);
	if (z == 0) {
		EnableWindow(GetDlgItem(wh, IDC_REMOVE), false);
		EnableWindow(GetDlgItem(wh, IDC_EXECUTE), false);
	}
}