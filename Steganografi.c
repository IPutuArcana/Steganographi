#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	
	char buff[54];
	char buffd;
	int width, menu, i = 0;
	int counter = 0;
	unsigned char pixel;
	FILE *file1, *file2;
	char pesan[1024];
	int padding;
	
//	Menu
	printf("Menu\n 1.Masukan Pesan\n 2.Ekstrak Pesan\nPilih Menu:\n");
	scanf("%i", &menu);
	
	switch(menu){
		case 1:
			//	memasukan pesan
				printf("masukan pesan: ");
				scanf("%s", pesan);
			
			//	menyisipkan pesan start
				file1 = fopen("Garou1.bmp", "rb");
				file2 = fopen("Garou2.bmp", "wb");
				
			//	copy header
				fread(buff, 1, 54, file1);
				fwrite(buff, 1, 54, file2);
				
			//	ukuran padding
				fseek(file1, 18, SEEK_SET);
				fread(&width, 4, 1, file1);
				padding = ( 4 - ((width * 3) %4) ) % 4;
				
			//	penyisipan ke pixel
				fseek(file1, 54, SEEK_SET);	
				if(pesan[i] != '\0'){
					for(int j=0;j<7;j++){
						
						fread(&pixel, sizeof(unsigned char), 1, file1);
						if(pesan[i] & 0x80){
							pixel |= 0x01;
						}else{
							pixel &= 0xFE;
						}
						pesan[i] = pesan[i] << 1;
						fwrite(&pixel, sizeof(unsigned char), 1, file2); counter++;

						// baca dan tulis padding
						if(counter == (width*3)){
							fread(&pixel, sizeof(unsigned char), padding, file1);
							fwrite(&pixel, sizeof(unsigned char), padding, file2);
							counter = 0;
						}
					}
					i++;
				}
				
			//	pembatasan dengan '\0'
				for(int z = 0;z<7;z++){
					fread(&pixel, sizeof(unsigned char), 1, file1);
					pixel &= 0xFE;
					fwrite(&pixel, sizeof(unsigned char), 1, file2); 
				}
				
			//	sisa yang belum ter-copy
				while(fread(&buffd, 1, 1, file1)){
						fwrite(&buffd, 1, 1, file2);
				}
				fclose(file1);
				fclose(file2);
				
			//	menyisipkan pesan end
			
			break;
		case 2:
			
			file2 = fopen("Garou2.bmp", "rb");
			
		//	ukuran padding
			fseek(file2, 18, SEEK_SET);
			fread(&width, 4, 1, file2);
			padding = ( 4 - ((width * 3) %4) ) % 4;
			

			
			fseek(file2, 54, SEEK_SET);
			
			while(1){
        		pesan[i] = 0;  // Inisialisasi pesan[i] ke 0
        		for (int j = 0; j < 7; j++) {
            		fread(&pixel, sizeof(unsigned char), 1, file2);
            		pesan[i] |= (pixel & 0x01);
            		
					if (j < 7) {  // Shift left 7 times, not the 8th time
                		pesan[i] = pesan[i] << 1;
            		}
            		
					counter++;
					if (counter == (width * 3)) {
                		fseek(file2, padding, SEEK_CUR);
                		counter = 0;
            		}
        		}
        		
        		printf("%c", pesan[i]);
				if (pesan[i] == '\0') {
            		break;
        		}
        		
        		i++;
    		}
			
			fclose(file2);
			printf("\n\n TEST CONDITION\n\n");
			break;
		default: 
		printf("pilihlah angka menu yang ada");
	}
	


	
	return(0);
}
