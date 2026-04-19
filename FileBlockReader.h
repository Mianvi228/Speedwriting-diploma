#ifndef FILEBLOCKREADER_H
#define FILEBLOCKREADER_H

#include <QObject>
#include <QString>
#include <QFile>

template <size_t blockSize>
class FileBlockReader : public QObject
{
private:
    QFile _inputFile;
    quint64 _fileOffset;
    bool _isEof = true;
    QByteArray _buffer = "";

public:
    void selectFile(QString &filename);
    void readBlock();
    bool isEof();
    const QByteArray& getBlock();
    void resetFile();

};

template <size_t blockSize>
void FileBlockReader<blockSize>::selectFile(QString &filename)
{
    _isEof = false;
    _fileOffset = 0;
    _inputFile.setFileName(filename);
}

template <size_t blockSize>
void FileBlockReader<blockSize>::readBlock()
{
    if (_isEof)
        return;
    if (_inputFile.fileName().isEmpty())
    {
        _isEof = true;
        return;
    }
    if (!_inputFile.open(QIODevice::ReadOnly))
    {
        _isEof = true;
        return;
    }
    _inputFile.seek(_fileOffset);
    _buffer = _inputFile.read(blockSize);
    if (_inputFile.atEnd())
        _isEof = true;
    else
        _fileOffset += _buffer.length();
    _inputFile.close();
}

template <size_t blockSize>
bool FileBlockReader<blockSize>::isEof()
{
    return _isEof;
}

template <size_t blockSize>
const QByteArray& FileBlockReader<blockSize>::getBlock()
{
    return _buffer;
}

template <size_t blockSize>
void FileBlockReader<blockSize>::resetFile()
{
    _fileOffset = 0;
    _isEof = false;
}

#endif // FILEBLOCKREADER_H
