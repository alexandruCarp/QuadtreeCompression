typedef struct Pixel{
    unsigned char r,g,b;
}Pixel;

typedef struct QuadtreeNode{
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
} __attribute__((packed)) QuadtreeNode;

typedef struct qTree{
    Pixel pix;
    unsigned int size;
    struct qTree *topL,*topR,*botL,*botR;
} *QTree;
