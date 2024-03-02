#ifndef N3_H
#define N3_H

#include "abstractdevice.h"
#include <QQueue>
#include <QVector>

#include "n3_constants.h"

class Neptune;
class QMutex;

struct queue_command
{
    N3Commands m_command;
    quint32 m_address;
    quint32 m_length;
    quint16 m_delay_ms;
    char *m_bytes_to_write;
};


class WorkerKeepAlive : public QObject
{
    Q_OBJECT
public:
    explicit WorkerKeepAlive();
    ~WorkerKeepAlive();
    void setThread(QThread *thread) { this->thread = thread; }
    int get_n_keeps() const;
    void receiveAck();

private:    
    void keepAlive();
    QByteArray keep_alive_command;

    bool working = false;
    QThread *thread = nullptr;
    int n_keeps = 0;
    std::unique_ptr<QMutex> mutex;

protected:
    friend class Neptune;
signals:
    void sendPacket(QByteArray packet, const uint delayms = 0);
    void finished();

public slots:
    void process();
    void stop();
    void start();
    void clear();
};



//======================================================================================================
struct s_key_item{
    char offset;
    bool absolute_value;
};



class Neptune : public AbstractDevice
{
    Q_OBJECT
public:    
    explicit Neptune(QString portName, QObject *parent);

    virtual  ~Neptune() override;

    virtual const QString &deviceName() const override;
    virtual void open() override;
    virtual const QString getSerialNumber() const override;
    virtual void set_date_time(const QDateTime& a_datetime) override;
    virtual int revision() const override { return m_software_revision; }
    virtual int product_type() const override { return static_cast<int>(m_product_type); }

    void setCorrectDateKoeff(int value) { m_correct_date_koeff = value; }

protected:
    void setupComPort() override;
    void executeCommand(N3Commands command, unsigned int address = 0, unsigned int length = 0, char *wbytes = nullptr, quint16 delay_ms = N3Constants::DelayBetweenCommands);
    virtual std::unique_ptr<QByteArray> jump_at(uint index) const override;
    std::unique_ptr<CustomJump> jump_from_raw(uint index) const;
    int m_software_revision{0};
    N3Types m_product_type = N3Types::Unknown;
    N3CommTypes m_commutication_type = N3CommTypes::Unknown;

    mutable QByteArray rawDataDetails;
    QByteArray rawDateTime;

    volatile unsigned int m_NumBlocks{0};

private:
    void init();
    void setAckBuffer(const QByteArray &data);
    void processType0Record(const QByteArray &data);
    void processReadMemory(const QByteArray &data);
    void processWriteMemory(const QByteArray &data);
    void processReadDateTime(const QByteArray &data);    
    void processDefault(const QByteArray &data);
    void encrypt(uint32_t &dword0, uint32_t &dword1, uint8_t NumRounds) const;
    void decrypt(uint32_t &dword0, uint32_t &dword1, uint8_t NumRounds) const;
    std::unique_ptr<QByteArray> cryptPacket(const QByteArray &packet, bool b_encrypt) const; //std::unique_ptr чтобы конструктор копирования QByteArray не вызывался
    std::unique_ptr<QByteArray> makeSingleByteCommand(const N3Commands command) const;
    char calculateChecksum(const QByteArray &packet) const;
    bool verifyType0Record();
    void setEncryptionKey(const QVector<s_key_item> &vec_key);
    bool checkAcknowledgment (const QByteArray &buffer, const bool single = false) const;
    QByteArray* getRawData(unsigned int address) const;
    unsigned int n_jumps_readed() const;
    void addJumpsToVector();
    void sendLastCommand();
    bool checkInBuffer();
    void dequeueCommand();

    QVector<s_key_item> ct3_key{{78, true}, {6, false}, {24, false}, {22, false}, {4, false}, {23, false}, {21, false}, {11, false}, {8, false}, {117, true}, {5, false}, {20, false}, {7, false}, {9, false}, {126, true}, {19, false}};
    QVector<s_key_item> ct4_key{{56, true}, {21, false}, {4, false}, {11, false}, {22, false}, {20, false}, {10, false}, {-103, true}, {5, false}, {6, false}, {8, false}, {-49, true}, {7, false}, {9, false}, {24, false}, {23, false}};
    QVector<s_key_item> ct5_key{{-86, true}, {21, false}, {4, false}, {11, false}, {22, false}, {20, false}, {10, false}, {105, true}, {5, false}, {6, false}, {8, false}, {68, true}, {7, false}, {9, false}, {24, false}, {23, false}};

    std::unique_ptr<QByteArray> outBuffer;
    QByteArray inBuffer;
    QByteArray ackBuffer;

    mutable QByteArray* rawData = nullptr;
    uint16_t memory_block_length{0};
    QQueue<queue_command> m_commands;

    uint32_t m_key[4]{0, 0, 0, 0};
    QByteArray Type0Record;
    uint type0Size = 0;    
    int m_correct_date_koeff = 0;

    queue_command last_command{N3Commands::None, 0, 0, 0, nullptr};

    QThread keep_alive_thread;
    WorkerKeepAlive keep_alive_worker;        

private slots:
    void finishedWriteData();
    virtual void processData(QByteArray data) override;

    void slotConnected() override;
    void slotReady() override;
    void slotReadyExit() override;
    void slotReceiving() override;
    void slotDisconnected() override;
    void slotProcessing() override;

signals:
};

#endif // N3_H
