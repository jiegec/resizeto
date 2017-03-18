#include <stdio.h>
#include <IL/il.h>
#include <fstream>

std::ifstream::pos_type filesize(const char* filename)
{
  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
  return in.tellg();
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: resizeto [filename] [size in kb]\n");
    return 0;
  }

  ilInit();
  ilEnable(IL_FILE_OVERWRITE);
  if (!ilLoadImage(argv[1])) {
    printf("Load image failed.");
  }

  const char *tmpFile = tmpnam(NULL);
  long targetSize = atoi(argv[2]) * 1024;
  long small=0, big=99, near=50, diff=0x7ffffff;
  while(small+1 < big) {
    long mid = (small+big) >> 1;
    printf("Try quality %ld\n", mid);
    ilSetInteger(IL_JPG_QUALITY, mid);
    ilSave(IL_JPG,tmpFile);
    long fileSize = filesize(tmpFile);
    if (fileSize <= targetSize) {
      small = mid;
    } else if (fileSize > targetSize) {
      big = mid;
    }
    int curDiff = std::abs(fileSize - targetSize);
    if (curDiff < diff){
      near = mid;
      diff = curDiff;
    }
  }
  std::remove(tmpFile);

  ilSetInteger(IL_JPG_QUALITY, small);
  ilSaveImage("output_less.jpg");
  ilSetInteger(IL_JPG_QUALITY, near);
  ilSaveImage("output_near.jpg");
  ilSetInteger(IL_JPG_QUALITY, big);
  ilSaveImage("output_more.jpg");
  return 0;
}
