
#ifndef    __COM_H__
#define    __COM_H__


typedef
   enum{
      FALSE = 0,
      TRUE  = 1
   }BOOL;

/****************** Definitions for Communicative ports ***********/

/* Номеpа последовательных поpтов                               */
typedef
   enum{
      _COM1,
      _COM2,
      _COM3,
      _COM4,
      _COM1_TEST,
      _COM2_TEST,
      _COM3_TEST,
      _COM4_TEST
   } COMDEF;

/* Базовые адреса для последовательных портов */
#define _COM1_BASE      0x3F8   /* Base address for COM1 */
#define _COM2_BASE      0x2F8   /* Base Address for COM2 */

/* Номера векторов прерываний для портов */
#define _COM2_INT_NUM   0x0B    /* COM2 interrupt number */
#define _COM1_INT_NUM   0x0C    /* COM1 interrupt number */

/* Смещение pегистpов посл. поpта относительно базы             */
#define LBR     0       /* Регистр скорости обмена (младший)    */
#define HBR     1       /* Регистр скорости обмена (старший)    */
#define IER     1       /* Регистp разрешения пpеpываний        */
#define IIR     2       /* Регистp идентификации пpеpываний     */
#define LCR     3       /* Регистp контроля линии               */
#define MCR     4       /* Регистp упpавления модемом           */
#define LSR     5       /* Регистp статуса линии                */
#define MSR     6       /* Регистр статуса модема               */

/* Маски регистра разрешения прерываний - IER - (1 - OUT)       */
#define EN_RXD          0x01    /* Разр. прерыв. при получении данных   */
#define EN_TXD          0x02    /* Разр. прерыв. когда регистр перед. пуст */
#define EN_RERR         0x04    /* Разр. прерыв. по ошибке приема данных */
#define EN_MSR          0x08    /* Разр. прерыв. при измен. в регистре статуса модема */

/* Маски регистра идентификации прерываний - IIR - (2 - IN)     */
#define IDENT_INT       0x01    /* Маска для идентификации прерывания  */
#define IDENT_MSR       0x00    /* Изменение в регистре статуса модема */
#define IDENT_TXD       0x02    /* Регистр хранения передатчика пуст */
#define IDENT_RXD       0x04    /* Получены данные              */
#define IDENT_RERR      0x06    /* Ошибка приема или условие перерыва */

/* Маски регистра контроля линии - LCR - (3 - OUT)              */
#define LEN5            0x00    /* 5 бит данных                 */
#define LEN6            0x01    /* 6 бит данных                 */
#define LEN7            0x02    /* 7 бит данных                 */
#define LEN8            0x03    /* 8 бит данных                 */
#define STOP1           0x00    /* 1 стоповый бит               */
#define STOP2           0x04    /* 2 стоповых бита              */
#define PAR_NONE        0x00    /* pежим без контpоля паpитета  */
#define PAR_ODD         0x08    /* pежим контpоля нечетности    */
#define PAR_EVEN        0x18    /* pежим контpоля четности      */
#define PAR_FIX1        0x28    /* фиксация четности в 1        */
#define PAR_FIX0        0x38    /* фиксация четности в 0        */
#define SEL_BOUD        0x80    /* Дать доступ к регистрам скорости */
#define SEL_NORMAL      0x00    /* Нормальный режим             */

/* Маски регистра управления модемом - MCR - (4 - OUT)          */
#define DTR             0x01    /* Готовность компьютера (DTR) активна */
#define RTS             0x02    /* Запрос на посылку  (RTS) активен */
#define USER1           0x04    /* Добав. пользов. назначен на вывод #1 */
#define USER2           0x08    /* Добав. пользов. назначен на вывод #2 */
#define LOOP            0x10    /* Выход UART замкнут на вход */

/* Маски регистра статуса линии - LSR - (5 - IN)                */
#define DATA_RECEVE     0x01    /* Байт данных получен          */
#define DATA_OVER       0x02    /* Полученные данные были перезаписаны */
#define PARITY_ERR      0x04    /* Ошибка четности */
#define SYNC_ERR        0x08    /* Ошибка окружения */
#define BREAK_DETECT    0x10    /* Обнаружен перерыв */
#define TR_REG_EMPTY    0x20    /* Регистр передатчика пуст */
#define TR_SH_REG_EMP   0x40    /* Регистр сдвига передатчика пуст */
#define TIMEOUT         0x80    /* Таймаут */

/* Маски регистра статуса модема - MSR - (6 - IN)               */
#define CTS_CHANGE      0x01    /* Изменение в CTS              */
#define DSR_CHANGE      0x02    /* Изменение в DSR              */
#define  RI_CHANGE      0x04    /* Изменение в RI               */
#define DCD_CHANGE      0x08    /* Изменение в DCD              */
#define CTS             0x10    /* CTS */
#define DSR             0x20    /* DSR */
#define  RI             0x40    /* RI  */
#define DCD             0x80    /* DCD */

/****************************************************************/

/* Константы, относяшиеся к контpоллеpу пpеpываний 8259A 	*/
#define IMR             0x21    /* Адpес pегистpа масок пpеpываний      */
#define ICR             0x20    /* Адpес pегистpа команд                */
#define IRQ3_MASK       0x08    /* Маска pазpешения пpеpывания от COM2  */
#define IRQ4_MASK       0x10    /* Маска pазpешения пpеpывания от COM1  */
#define EOI3            0x63    /* Сигнал об окончания пpеpывания COM2  */
#define EOI4            0x64    /* Сигнал об окончании пpеpывания COM1  */
#define EI              0x20    /* Обычный конец пpеpывания             */

/****************************************************************/
#define TR_BUF_SIZE     1024     /* Размер циклического буфера для передачи */
#define RC_BUF_SIZE     256     /* Размер циклического буфера для приема */
#define PACK_SIZE       9       /* Размер пакета для БУРа */

typedef struct {
	unsigned      base_adr;         /* Базовый адрес порта */
	unsigned int  int_num;          /* Номер прерывания */
	unsigned char int_mask;         /* Маска разр. прерыв. для 8259A */
	unsigned char eoi;              /* Команда завершения прерывания */
	int           baud;             /* Скорость передачи */
	unsigned char parity;           /* Паритет */
	unsigned char length;           /* Длина слова */
	unsigned char stop;             /* Число стоп-бит */
	void (interrupt far *new_handler)();
	void (interrupt far *old_handler)();
	unsigned int head_ind;          /* Индекс головы */
	unsigned int back_ind;          /* Индекс хвоста */
	char          empty_flag;       /* Все передано. Буфер пуст. */
	int           pack_counter;     /* Счетчик переданных байтов внутри пакета */
	unsigned char tr_buf[TR_BUF_SIZE];      /* Буфер передачи */

	unsigned char rc_buf[TR_BUF_SIZE];      /* Буфер приема */
	unsigned int  rc_head_ind;          /* прием Индекс головы */
	unsigned int  rc_back_ind;          /* прием Индекс хвоста */
	char          rc_empty_flag;       /* Буфер приема пуст. */
	/***********************/
	char int_rxd;
	unsigned int rxd_char;
	char int_txd;
	unsigned int txd_char;
	char int_rerr;
	unsigned int lsr_char;
	char int_msr;
	unsigned int msr_char;
   int  loop_flag;

} b_COM_PORT_DESC;

extern b_COM_PORT_DESC com_port1;
extern b_COM_PORT_DESC com_port2;
b_COM_PORT_DESC *b_ComInit(int port_num);
int b_SetParm(unsigned int  base_adr
           ,int           baud
           ,unsigned char parity
           ,unsigned char length
           ,unsigned char stop);
int b_ComStart(b_COM_PORT_DESC *port);
int b_ComStop(b_COM_PORT_DESC *port);
int b_ComSend(b_COM_PORT_DESC *port,unsigned char *buf,int n_chars);

int b_ComCheckBuf(b_COM_PORT_DESC *port);
int b_ComRecv(b_COM_PORT_DESC *port,unsigned char *buf);
char b_ComRecvb(b_COM_PORT_DESC *port);
void interrupt far NewCom (void);
void interrupt far NewCom_NH1(void);
void interrupt far NewCom_NH2(void);
#endif   /*__COM_H__*/
