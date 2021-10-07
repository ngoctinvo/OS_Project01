#include "syscall.h"
char str[256];

int main(){
PrintString("\n Hello cac ban \n Moi cac ban nhap mot chuoi: ");
ReadString(str,255);
PrintString("\n Chuoi vua nhap la: ");
PrintString(str);

Halt();
}
