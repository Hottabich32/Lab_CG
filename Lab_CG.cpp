#include <iostream>
#include "Image.h"

int main()
{
    std::string inPath, outPath;
    double scale;

    std::cout << "Путь к исходному изображению (png/jpg): ";
    std::getline(std::cin, inPath);

    Image src;
    if (!src.load(inPath)) {
        std::cout << "Не удалось загрузить изображение (проверьте путь и формат: png/jpg).\n";
        return 1;
    }
    std::cout << "Загружено: " << src.getWidth() << "x" << src.getHeight() << "\n";

    std::cout << "Коэффициент увеличения: ";
    std::cin >> scale;
    if (scale <= 0) {
        std::cout << "Коэффициент должен быть положительным.\n";
        return 1;
    }

    std::cin.ignore();

    std::cout << "Путь для сохранения результата (.png/.jpg): ";
    std::getline(std::cin, outPath);

    int newW = (int)(src.getWidth() * scale + 0.5);
    int newH = (int)(src.getHeight() * scale + 0.5);

    Image dst;
    dst = src.bilinearInterpolation(newW, newH);
   
    if (!dst.save(outPath)) {
        std::cout << "Ошибка сохранения файла (проверьте расширение: png/jpg).\n";
        return 1;
    }
    std::cout << "Готово: " << dst.getWidth() << "x" << dst.getHeight()
        << " сохранено в " << outPath << "\n";
    return 0;
}
