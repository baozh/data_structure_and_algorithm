// class for factory pattern
enum ImageType{
    GIF,
    JPEG
};

class ImageReader {
    // implementation for image reader base class
};

class GIFReader : public ImageReader {
    // implementation for GIF reader derived class
};

class JPEGReader : public ImageReader {
    // implementation for JPEG reader derived class
};

class ImageReaderFactory {
public:
    static ImageReader *imageReaderFactoryMethod(ImageType imageType) {
        ImageReader *product = NULL;

        switch (imageType) {
            case GIF:
                product = new GIFReader();
            case JPEG:
                product = new JPEGReader();
                //...
        }
        return product;
    }
};
