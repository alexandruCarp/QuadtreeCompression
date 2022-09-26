Pixel** readPPM(char *filename,int *width,int *height);
void compress(char *filenameIN,char *filenameOUT, int threshold);
void free_tree(QTree root);
void free_matrix(Pixel **img,int size);
void decompress(char *filenameIN, char *filenameOUT);
QTree buildTree(Pixel **image, int size, int x, int y,unsigned int threshold, uint32_t *numar_culori, uint32_t *numar_noduri);
void buildImage(Pixel **image, int x, int y, QTree nod);
void mirror(char type, char *filenameIN, char *filenameOUT,int threshold);