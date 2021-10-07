/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

int A[1024];	/* size of physical memory; with code, we'll run out of space!*/

int
main()
{
	int i, j, tmp,z;
	PrintString("\nCHUONG TRINH SORT");
	PrintString("\n-------------------------------------------");
	PrintString("\nNhap so nguyen n - so phan tu cua mang");
	n=ReadInt();

	for (i=0;i<n;i++)
	{
	PrintString("Nhap A[");
	PrintInt(i);
	PrintString("] = ");
	A[i]=ReadInt();
	}

	PrintString("\nSap xep mang theo thu tu tang dan (nhap 1) hay giam dan (nhap 0)?");
	z=ReadInt();

	if (z==1){
		for (i=0;i<n-1;i++){
			for(j=0;j<n-i-1;j++){
				if (A[j] > A[j+1]){
					tmp=A[j];
					A[j]=A[j+1];
					A[j+1]=tmp;
				}
			}
		}
	else{
		for (i=0;i<n-1;i++){
			for(j=0;j<n-i-1;j++){
				if (A[j] < A[j+1]){
					tmp=A[j];
					A[j]=A[j+1];
					A[j+1]=tmp;
				}
			}
		}

	}
	
	PrintString("\nMang sau khi sap xep: \n ");
	for (i=0;i<n;i++){
		PrintInt(A[i]);
		PrintString("  ");
	}

	Halt();
}

	
