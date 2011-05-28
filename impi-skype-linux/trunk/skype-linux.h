/*
 * skype-linux.h
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#ifndef SKYPELINUX_H_
#define SKYPELINUX_H_

#include <QDateTime>
#include <QDir>
#include <QMap>
#include <QVector>
#include "../impi/plugin-interface.h"

namespace SkypeLinux {

// TODO(ashl1future): realize error report system with error status. If already have error status, error status
// will not be changed to apply only the first error. May realize Error() method that set check if error status is set
// and will set if is not already set. Then check error_status only one on the main method NextMessages(). Add comments that
// if returning value is false, check error() function.
//
// The class guarantee that messages sorted by date
// Skype block - the block started from main_header_char_ and finished at other main_header_char_ or end of file.
class PluginSkypeLinux: public QObject, public PluginInterface {
Q_OBJECT
Q_INTERFACES(PluginInterface)

public:
	PluginSkypeLinux(QObject *parent = 0);
	// TODO(ashl1future): should execute ObjectsClean()
	virtual ~PluginSkypeLinux();

	virtual bool can_init_from_file() const;
	virtual QString full_name() const;
	virtual QString GetHomePath() const;
	// The function find for messages from 'time_from' (included 'time_from') and not exceeded 'max_messages_size' size in bytes
	// and set it in 'message' argument after clear.
	// If cannot return at least one message due the small 'max_message_size' generate the error with according status.
	// It means that the error status will be set if the next message after or at 'time_from'
	// cannot be returned due 'max_message_size'.
	// The method also set 'last_message_time' to the last returned message datetime.
	// Method guarantee that messages sorted by date ascending (if dates the same the will be in any order).
	// Method guarantee that no messages with date before the last returned message date returned kept in the history files,
	// so it's correct to next execute method with 'time_from' set to the last returned message date.
	// Method delete messages (pointer data) previously method set in 'messages', so the main program mustn't delete it
	// manually but execute this method again or execute ObjectsClean() method. So:
	// After next executing of this method PluginMessage pointers previously returned will be incorrect. Pointers to
	// other PluginObjects may be available but it's not recommended to use them because they could be incorrect any time.
	// 'last_message_time' set at the end of the method, so it's ok to use the same variable for 'time_from'
	// and 'last_message_time'.
	//
	// Return true if successful.
	// time_from - which time the messages should be from
	// max_messages_size - the max size of all messages.
	// last_message_time - the output variable. The
	// messages - the output variable. Should be empty, because class don't delete allocated data and
    virtual bool GetNextMessagesOrDie(const QDateTime& time_from, quint64 max_messages_size, QDateTime& last_message_time,
    		QList<PluginMessage*>& messages);
	virtual void GetUserAccounts(const QDir& path, QVector<QDir>& pathes, QVector<QString>& names) const;
    virtual QString major_name() const;
	virtual QString minor_name() const;
    // This method clean all PluginObjects pointers data such as PluginMessage, PluginChat etc. After executing this function
    // old pointers will incorrect.
    // Run this method carefully.
    // TODO(ashl1future)
	virtual void ObjectsClean();
	virtual QString version() const;
	virtual void working_dir(const QString& path);
	virtual void working_dir(const QDir& dir);

private:
    // This method read date and time (4 bytes) from the string.
	// Return true if successful.
    // source - the QByteArray source string
	// from - where start read from
	// datetime - datetime variable the method write read datetime to
	static void ReadDateTime(const QByteArray& source, quint32 from, QDateTime& datetime);
    // This method read utf8 string from the source until "\0" terminated symbol.
	// Return true if successful.
	// source - the QByteArray source string
    // from - where start read from
	// string - string variable the method write read string to
	static bool ReadUtf8StringOrDie(const QByteArray& source, quint32 from, QString& string);

	static const bool can_init_from_file_;
	static const QString full_name_;
	static const QString major_name_;
	static const QString minor_name_;
	static const QString version_;

    static const quint8 kBodyHeaderLength = 3;
    // Header for body: 0x03, 0xfc, 0x03
    static const char body_header_char_[kBodyHeaderLength];
    static const QByteArray body_header_;
    static const quint8 kChatFileChatIdHeaderLength = 4;
	// Header for chat id in chat*.dbb files: 0x00, 0x03, 0xb8, 0x03
	static const char chat_file_chat_id_header_char_[kChatFileChatIdHeaderLength];
	static const QByteArray chat_file_chat_id_header_;
    static const quint8 kChatmsgFileChatIdHeaderLength = 3;
    // Header for chat id in chatmsg*.dbb files: 0x03, 0xe0, 0x03
    static const char chatmsg_file_chat_id_header_char_[kChatmsgFileChatIdHeaderLength];
    static const QByteArray chatmsg_file_chat_id_header_;
    static const quint8 kDateHeaderLength = 3;
    // Header for date and time (in own Skype format, read ReadDateTime method inside): 0x00, 0xe5, 0x03
    static const char date_header_char_[kDateHeaderLength];
    static const QByteArray date_header_;
    static const quint8 kMainHeaderLength = 4;
    // The main Skype header any block started (see Skype block): 0x6c, 0x33, 0x33, 0x6c
    static const char main_header_char_[kMainHeaderLength];
    static const QByteArray main_header_;
    static const quint8 kParticipantHeaderLength = 4;
	// The header for the sender login: 0x04, 0x03, 0xcc, 0x03
	static const char participant_header_char_[kParticipantHeaderLength];
	static const QByteArray participant_header_;
    static const quint8 kSenderLoginHeaderLength = 4;
    // The header for the sender login: 0x04, 0x03, 0xe8, 0x03
    static const char sender_login_header_char_[kSenderLoginHeaderLength];
    static const QByteArray sender_login_header_;

    // This method move stream after any of 2 headers, specified in arguments.
    // This method don't leave the Skype block.
    // Now it just only find it by Finite State Machine and not skip any byte.
    // Return true if successful.
    // stream - the stream method operate with
	// length[0, 1] - the length in bytes of the header
	// header[0, 1] - the header inside Skype block the stream will be moved to
    // found - represent the number of the header was found: -1 - not found (and return false),
    //  0 - found header0, 1 - found header1
    //bool MoveToAnyHeaderOrDie(QDataStream& stream, quint8 length0, const char header0[],

    // Allocate if needed and return pointer to the PluginClient.
	PluginClient* client();
	// Allocate if needed and set 'account' to the user pointer according 'login_name'
	// Return true if successful.
	// login_name - the login name
	// account - the output variable of the got PluginAccount pointer
	bool GetAccountOrDie(const QString& login_name, PluginAccount*& account);
	// This method find queried ChatSkypLinux in chats_ and not find read chat information from chat#.dbb files,
	// allocate ChatSkypeLinux and set the pointer to it and add to chats_.
	// Return true if successful.
	// chat_id - the id of the chat (like #user_login1/$user_login2;7e6a242bb833615)
	// chat_skype_linux - the non allocated point to the chat_skype_linux method will allocate to
	bool GetChatOrDie(const QString& chat_id, PluginChat*& chat_skype_linux);
	// Method find for the earlist messages in messages_dates_ (not include messages with QDateTime means no message)./
	// Return the list of numbers in any messages_*_ list for messages with the earliest date.
	// Return the empty list if all times is dummy QDateTime()
	QList<quint32> GetNumbersEarliestMessages() const;
	// Allocate if needed and set 'user' to the user pointer according 'login_name'
	// Return true if successful.
	// login_name - the login name
	// user - the output variable of the got PluginUser pointer
	bool GetUserOrDie(const QString& login_name, PluginUser*& user);
	// Initialize lists of absolute filenames for .dbb files for all types (chats, messages, users etc) and lists
	// with numbers of Skype block data in these files.
	// Should be executed once every GetNextMessagesOrDie method to update lists.
	// example: absolute_directory_path/chat256.dbb, absolute_directory_path/chatmsg4096.dbb,
	// absolute_directory_path/user16384.dbb
	// The separator in filename is depend on running OS, so it's correct.
	// Return true if successful.
	bool InitAbsoluteFilenamesOrDie();
	// Initialize messages_*_ structures
	// Return true if successful.
	bool InitMesssagesStructuresOrDie();
	// Clean messages_to_export_ (delete PluginMessage objects and just only them, not any PluginObjects)
	// This method should be run at the beginning GetNextMessagesOrDie() method
	// TODO(ashl1future)
	void MessagesClean();
	// Search and read the message from 'time_from' datetime (included 'time_from') from the stream
	// and accordingly set messages_dates_. Fill messages_ structures.
	// The stream should be at Main header position.
	// If not found any message from current stream position with datetime >= time_from, set messages_ to NULL,
	// messages_dates_ to dummy QDateTime and messages_body_sizes_ to 0.
	// Method don't delete *message but allocate new!
	// Return true if no error present. If not found with specified time_from, return also true (see 'message' description)!
	// number_in_vector_ - the number in the messages_* vectors the operations according to
	// time_from - the datetime when messages will be searched from
	bool ReadMessageOrDie(quint32 number_in_vector_,const QDateTime& time_from);
	// Allocate if needed and return pointer to the PluginProtocol.
	PluginProtocol* protocol();
	// Uninitialize messages structures except messages_to_export_ (it should be cleaned at next GetNextMessagesOrDie() run
	// by MessagesClean() method)
	void UninitMessagesStructures();

	// The map of the PluginAccount identified by login name
	QMap<QString, PluginAccount*> accounts_;
	// The map of the PluginChat identified by its ID (like #user_login1/$user_login2;7e6a242bb833615)
	QMap<QString, PluginChat*> chats_;
	// The list of absolute filenames to the chat*.dbb files
	QStringList chat_absolute_filenames_;
	// The size of the Skype block (except 4 byte in Skype header and also 4 byte iterator?)
	// according to chat_absolute_filenames_
	QList<quint32> chat_block_sizes_;
	// The pointer to the PluginClient. For the Skype it's only one client possible.
	// Should be initialized by NULL at the constructor.
	// The value: Skype Linux
	PluginClient* client_;

	// The current message in according to messages_absolute_filenames_
	QList<PluginMessage*> messages_;
	// The datetime of the current message in according to messages_absolute_filenames_
	QList<QDateTime> messages_dates_;
	// The list of absolute filenames to the chatmsg*.dbb files
	QStringList messages_absolute_filenames_;
	// The size of the Skype block (except 4 byte in Skype header and also 4 byte iterator?)
	// in according to messages_absolute_filenames_
	QList<quint32> messages_block_sizes_;
	// The utf8 symbols count in messages body in according to messages_absolute_filenames_
	QList<quint64> messages_body_sizes_;
	// The chatmsg*.dbb files in according to messages_absolute_filenames_
	QList<QFile*> messages_files_;
	// The streams in according to messages_absolute_filenames_;
	// QDataStream* = 0 is guaranteed by QList in first creation
	QList<QDataStream*> message_streams_;

	// All messages that will be exported
	QList<PluginMessage*> messages_to_export_;

	// The pointer to the PluginProtocol. For the Skype it's only one protocol possible.
	// Should be initialized by NULL at the constructor.
	// The value: Skype
	PluginProtocol* protocol_;
	// The absolute directory where Skype files are located (example: /home/user/.Skype/user/)
	QDir working_dir_;
};
} // namespace SkypeLinux

#endif /* SKYPELINUX_H_ */
