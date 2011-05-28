/*
 * skype-linux.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#include <QDataStream>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QVector>

#include <QDebug>

#include <qplugin.h>
#include "skype-linux.h"

using ::SkypeLinux::PluginSkypeLinux;

const bool PluginSkypeLinux::can_init_from_file_ = false;
const QString PluginSkypeLinux::full_name_ = QString("Skype Linux");
const QString PluginSkypeLinux::major_name_ = QString("Skype");
const QString PluginSkypeLinux::minor_name_ = QString("Linux");
const QString PluginSkypeLinux::version_ = QString("0.1");

// TODO(ashl1future): use macros
const char PluginSkypeLinux::body_header_char_[kBodyHeaderLength] = {0x03, 0xfc, 0x03};
const QByteArray PluginSkypeLinux::body_header_ = QByteArray(body_header_char_, kBodyHeaderLength);
const char PluginSkypeLinux::chat_file_chat_id_header_char_[kChatFileChatIdHeaderLength] = {0x00, 0x03, 0xb8, 0x03};
const QByteArray PluginSkypeLinux::chat_file_chat_id_header_ = QByteArray(chat_file_chat_id_header_char_, kChatFileChatIdHeaderLength);
const char PluginSkypeLinux::chatmsg_file_chat_id_header_char_[kChatmsgFileChatIdHeaderLength] = {0x03, 0xe0, 0x03};
const QByteArray PluginSkypeLinux::chatmsg_file_chat_id_header_ =
		QByteArray(chatmsg_file_chat_id_header_char_, kChatmsgFileChatIdHeaderLength);
const char PluginSkypeLinux::date_header_char_[kDateHeaderLength] = {0x00, 0xe5, 0x03};
const QByteArray PluginSkypeLinux::date_header_ = QByteArray(date_header_char_, kDateHeaderLength);
const char PluginSkypeLinux::main_header_char_[kMainHeaderLength] = {0x6c, 0x33, 0x33, 0x6c};
const QByteArray PluginSkypeLinux::main_header_ = QByteArray(date_header_char_, kMainHeaderLength);
const char PluginSkypeLinux::participant_header_char_[kParticipantHeaderLength] = {0x04, 0x03, 0xcc, 0x03};
const QByteArray PluginSkypeLinux::participant_header_ = QByteArray(participant_header_char_, kParticipantHeaderLength);
const char PluginSkypeLinux::sender_login_header_char_[kSenderLoginHeaderLength] = {0x04, 0x03, 0xe8, 0x03};
const QByteArray PluginSkypeLinux::sender_login_header_ = QByteArray(sender_login_header_char_, kSenderLoginHeaderLength);

PluginSkypeLinux::PluginSkypeLinux(QObject *parent)
    : QObject(parent), client_(NULL), protocol_(NULL)
{
}

/*virtual */ PluginSkypeLinux::~PluginSkypeLinux(){
}

/*virtual */ bool PluginSkypeLinux::can_init_from_file() const {
	return can_init_from_file_;
}

/*virtual */ QString PluginSkypeLinux::full_name() const {
	return full_name_;
}

/*virtual */ QString PluginSkypeLinux::GetHomePath() const {
	// Because only in Linux
	QDir dir = QDir::home();
	dir.cd(".Skype");
	return dir.path();
}

/*virtual */ bool PluginSkypeLinux::GetNextMessagesOrDie(const QDateTime& time_from,
		quint64 max_messages_size, QDateTime& last_message_time, QList<PluginMessage*>& messages) {
	/* The solution:
	 *
	 * 1. 'date_last_exported_' = 'time_from'
	 * 2. Initialize messages_*_ structures and read the message from the stream until >= date_last_exported_.
	 * 3. Get the numbers in list of messages_*_ which are the earliest. It could be one or several.
	 * 4. Check the size not exceeded max_messages_size of all the messages from the previous step to be exported.
	 * 5. 'date_last_exported_' = the date of the earliest messages from step 3.
	 * 6. Export the messages from the step 3. - add to 'messages_to_export_'.
	 * 7. Read next message in streams contained exported messages.
	 * 8. Add messages size to 'exported_messages_size_'
	 * 9. Clean messages_*_ structures
	 *
	 */

	// TODO(ashl1future)
	//MessagesClean();

	if (messages.count() != 0) {
		// TODO(ashl1future): report error
		return false;
	}

	if (!working_dir_.exists())
		return false;

	// 2. Initialize messages_*_ structures
	if (!InitMesssagesStructuresOrDie()) {
		// TODO(ashl1ifuture): report error
		return false;
	}

	QList<quint32> numbers_earliest_messages;
	QDateTime date_last_exported_ = time_from;
	quint64 exported_messages_size_ = 0;

	// 2. Read the message from the stream until >= date_last_exported_
	quint32 i = 0;
	QList<QDataStream*>::iterator streams_iterator = message_streams_.begin();
	for (; streams_iterator != message_streams_.end(); ++i, ++streams_iterator) {
		if (!ReadMessageOrDie(i, date_last_exported_)) {
			// TODO(ashl1future): report error
			return false;
		}
	}

	bool can_export = true;
	bool first_time_run = true;

	while (can_export) {
		// 3. Get the numbers in list of messages_*_ which are the earliest. It could be one or several.
		numbers_earliest_messages = GetNumbersEarliestMessages();

		if (numbers_earliest_messages.count() == 0) {
			// cannot found messages with date >= 'time_from'
			can_export = false;
		} else {
			// 4. Check the size not exceeded max_messages_size of all the messages from the previous step to be exported.
			quint64 bodies_size = 0;
			for (QList<quint32>::const_iterator it = numbers_earliest_messages.constBegin();
					it != numbers_earliest_messages.constEnd(); ++it)
				bodies_size += messages_body_sizes_[*it];

			can_export = exported_messages_size_ + bodies_size <= max_messages_size;
			if (first_time_run && !can_export) {
				// TODO(ashl1future): report error
				return false;
			}
			if (can_export) {
				// 5. 'date_last_exported_' = the date of the earliest messages from step 3.
				date_last_exported_ = messages_dates_[numbers_earliest_messages.first()];
				// 6. Export the messages from the step 3. - add to 'messages_to_export_'.
				for (QList<quint32>::const_iterator it = numbers_earliest_messages.constBegin();
							it != numbers_earliest_messages.constEnd(); ++it) {
					messages_to_export_.append(messages_[*it]);
					// 7. Read next message in streams contained exported messages.
					if (!ReadMessageOrDie(*it, date_last_exported_)) {
						// TODO(ashl1future): report error
						return false;
					}
				}
				// 8. Add messages size to 'exported_messages_size_'
				exported_messages_size_ += bodies_size;
			}
		}
		first_time_run = false;
	}

	messages = messages_to_export_;
	last_message_time = date_last_exported_;

	qDebug() << messages.count() << " finished";
	qDebug() << chats_.count() << "chats";
	// 9. Clean messages_*_ structures
	UninitMessagesStructures();
	return true;
}

/*virtual */ void PluginSkypeLinux::GetUserAccounts(const QDir& path, QVector<QDir>& pathes,
		QVector<QString>& names) const {
	// check if the path is valid
	// should be files: "shared.lck" and "shared.xml"
	// directories are an accounts
	if (!path.exists("shared.lck") || !path.exists("shared.xml")){
		//throw Error(4, "The path is not valid path with accounts");
	}
	pathes.clear();
	names.clear();
	foreach(QString dirName, path.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
		names.append(dirName);
		QDir dir(path);
		dir.cd(dirName);
		pathes.append(dir);
	}
}

/*virtual */ QString PluginSkypeLinux::major_name() const {
	return major_name_;
}

/*virtual */ QString PluginSkypeLinux::minor_name() const {
	return minor_name_;
}

/*virtual */ void PluginSkypeLinux::ObjectsClean() {
	// TODO(ashl1future):
}

/*virtual */ QString PluginSkypeLinux::version() const {
	return version_;
}


/*virtual */ void PluginSkypeLinux::working_dir(const QString& path) {
	working_dir_ = QDir(path);
}


/*virtual */ void PluginSkypeLinux::working_dir(const QDir& dir) {
	working_dir_ = dir;
}

//bool PluginSkypeLinux::MoveToAnyHeaderOrDie(QDataStream& stream, quint8 length0, const char header0[],
//    		quint8 length1, const char header1[], quint8& found) const {
//	char c;
//	quint8 state0 = 0;
//	quint8 state1 = 0;
//	quint8 main_header_state = 0;
//	while (state0 != length0 && state1 != length1 && main_header_state != kMainHeaderLength && !stream.atEnd()) {
//		if (stream.readRawData(&c, 1) != 1) {
//			// TODO(ashl1future): report error
//			return false;
//		} else {
//			if (c == header0[state0])
//				++state0;
//			else
//				state0 = 0;
//
//			if (c == header1[state1])
//				++state1;
//			else
//				state1 = 0;
//
//			if (c == main_header_char_[main_header_state])
//				++main_header_state;
//			else
//				main_header_state = 0;
//		}
//	}
//	if (state0 == length0) {
//		found = 0;
//		return true;
//	} else if (state1 == length1) {
//		found = 1;
//		return true;
//	} else {
//		found = -1;
//		// TODO(ashl1future): report error
//		return false;
//	}
//}

void PluginSkypeLinux::ReadDateTime(const QByteArray& source, quint32 from, QDateTime& datetime) {
	quint32 d0 = (quint8)source[from] - 0x80;
	quint32 d1 = (quint8)source[from + 1] - 0x80;
	quint32 d2 = (quint8)source[from + 2] - 0x80;
	quint32 d3 = (quint8)source[from + 3] - 0x80;
	d1 = d1 << 7;
	d2 = d2 << 14;
	d3 = d3 << 21;
	// TODO(ashl1future): do we need to set time zone?
	// Skype starts time from 10 of January 2004 17:37:04
	datetime = QDateTime(QDate(2004, 01, 10), QTime(17, 37, 04), Qt::UTC);
	datetime = datetime.addSecs(d0 + d1 + d2 + d3);
}

bool PluginSkypeLinux::ReadUtf8StringOrDie(const QByteArray& source, quint32 from, QString& string) {
	qint32 endpos = source.indexOf((char)0x00, from);
	if (endpos < 0)
		return false;
	string = QString::fromUtf8(source.mid(from, endpos - from + 1).data(), endpos - from);
	return true;
}

PluginClient* PluginSkypeLinux::client() {
	if (!client_)
		client_ = new PluginClient("Skype Linux");
	return client_;
}

bool PluginSkypeLinux::GetAccountOrDie(const QString& login_name, PluginAccount*& account) {
	if (accounts_.constFind(login_name) == accounts_.constEnd()) {
		PluginUser* user = NULL;
		bool successful = GetUserOrDie(login_name, user);
		if (successful) {
			account = new PluginAccount(login_name, protocol_, user);
			accounts_[login_name] = account;
			return true;
		} else {
			return false;
		}
	}
	account = accounts_[login_name];
	return true;
}

QList<quint32> PluginSkypeLinux::GetNumbersEarliestMessages() const {
	QList<quint32> result;
	QDateTime earliest_datetime;
	QDateTime null_datetime;
	quint32 i = 0;
	while (i < (quint32)messages_dates_.count() && messages_dates_[i] == null_datetime)
		++i;
	if (i < (quint32)messages_dates_.count()) {
		earliest_datetime = messages_dates_[i];
		result.append(i);
	}
	for (++i; i < (quint32)messages_dates_.count(); ++i) {
		if (messages_dates_[i] == earliest_datetime) {
			result.append(i);
		} else if (messages_dates_[i] < earliest_datetime && messages_dates_[i] != null_datetime){
			earliest_datetime = messages_dates_[i];
			result.clear();
			result.append(i);
		}
	}
	return result;
}

bool PluginSkypeLinux::GetChatOrDie(const QString& chat_id, PluginChat*& chat_skype_linux) {
	if (chats_.constFind(chat_id) != chats_.constEnd()) {
		chat_skype_linux = chats_[chat_id];
		return true;
	} else {
		// TODO(ashl1future): fill time
		chat_skype_linux = new PluginChat(QDateTime(), QDateTime());
		chats_[chat_id] = chat_skype_linux;
		return true;
		// See comments at the end of commented block to understand what this code is for
//		QString chat_id_str;
//		QFile file;
//		quint8 header_num;
//		QStringList participants_list;
//		QString participants_str;
//		bool found = false;
//		QDataStream stream;
//		QString string;
//
//		QStringList filenames = InitAbsoluteFilenamesOrDie(kFileChat);
//		for (QStringList::const_iterator filename_iterator = filenames.constBegin();
//				filename_iterator != filenames.constEnd() && !found; ++filename_iterator) {
//			file.setFileName(*filename_iterator);
//			if (file.open(QIODevice::ReadOnly)) {
//				stream.setDevice(&file);
//				MoveToMainHeaderOrDie(stream);
//				while (!stream.atEnd() && !found) {
//					MoveToAnyHeaderOrDie(stream, kChatFileChatIdHeaderLength, chat_file_chat_id_header_char_,
//							kParticipantHeaderLength, participant_header_char_, header_num);
//					ReadUtf8StringOrDie(stream, string);
//					if (header_num == 0) {
//						// read chat id
//						if (string == chat_id) {
//							// found chat, read participants
//							MoveToHeaderOrDie(stream, kParticipantHeaderLength, participant_header_char_);
//							ReadUtf8StringOrDie(stream, participants_str);
//							participants_list = string.split(' ');
//							found = true;
//						}
//					} else if (header_num == 1) {
//						// read participants
//						MoveToHeaderOrDie(stream, kChatFileChatIdHeaderLength, chat_file_chat_id_header_char_);
//						ReadUtf8StringOrDie(stream, chat_id_str);
//						if (chat_id_str == chat_id) {
//							found = true;
//							participants_list = string.split(' ');
//						}
//					} else {
//						// TODO(ashl1future): report error
//						return false;
//					}
//					MoveToMainHeaderOrDie(stream);
//				}
//			} else {
//				// TODO(ashl1future): report error
//				return false;
//			}
//		}
//		if (!found) {
//			// TODO(ashl1future): report error: not found
//			return false;
//		} else {
//			// Why the code above was written? What is for? We just need to allocate new ChatSkypeLinux
//			// Maybe it'll be useful if program in the future will be organize state of chats: who in chat
//			// at the time, who is exited, who is entered. Now program just only collect messages.
//		}
	}
}

bool PluginSkypeLinux::GetUserOrDie(const QString& login_name, PluginUser*& user) {
	// TODO(ashl1future): read it from user*.dbb files
	user = new PluginUser(login_name, login_name, login_name);
	return true;
}

bool PluginSkypeLinux::InitAbsoluteFilenamesOrDie() {
	QRegExp regexp;

	chat_absolute_filenames_ = working_dir_.entryList(QStringList("chat[0-9]*.dbb"));
	if (chat_absolute_filenames_.count() == 0) {
		// TODO(ashl1future): report error
		return false;
	}
	regexp.setPattern("(\\d*)(.dbb)");
	for (QStringList::const_iterator it = chat_absolute_filenames_.constBegin();
			it != chat_absolute_filenames_.constEnd(); ++it) {
		regexp.indexIn(*it);
		chat_block_sizes_.append(regexp.cap(1).toULong());
	}
	chat_absolute_filenames_.replaceInStrings(QRegExp("^"), working_dir_.path() + working_dir_.separator());

	messages_absolute_filenames_ = working_dir_.entryList(QStringList("chatmsg[0-9]*.dbb"));
	if (messages_absolute_filenames_.count() == 0) {
		// TODO(ashl1future): report error
		return false;
	}
	regexp.setPattern("(\\d*)(.dbb)");
	for (QStringList::const_iterator it = messages_absolute_filenames_.constBegin();
			it != messages_absolute_filenames_.constEnd(); ++it) {
		regexp.indexIn(*it);
		messages_block_sizes_.append(regexp.cap(1).toULong());
	}
	messages_absolute_filenames_.replaceInStrings(QRegExp("^"), working_dir_.path() + working_dir_.separator());
	return true;
}

bool PluginSkypeLinux::InitMesssagesStructuresOrDie() {
	if (!InitAbsoluteFilenamesOrDie()) {
		// TODO(ashl1future): report error
		return false;
	}
	quint32 files_count = messages_absolute_filenames_.count();

	for (quint32 i = 0; i < files_count; ++i) {
		messages_.append(NULL);
		messages_dates_.append(QDateTime());
		messages_body_sizes_.append(0);
	}

	for (QStringList::const_iterator filename_iterator = messages_absolute_filenames_.constBegin();
			filename_iterator != messages_absolute_filenames_.constEnd(); ++filename_iterator) {
		QFile* file = new QFile(*filename_iterator);
		file->open(QIODevice::ReadOnly);
		messages_files_.append(file);

		QDataStream* stream = new QDataStream(messages_files_.last());
		message_streams_.append(stream);
	}
	return true;
}

bool PluginSkypeLinux::ReadMessageOrDie(quint32 number_in_vector_, const QDateTime& time_from) {
	char block_type;
	QString body;
	QString chat_id;
	QString sender_login;
	QByteArray skype_block;
	char skype_header[4];
	qint32 strpos;
	QDateTime time;

	char* skype_block_char = new char[messages_block_sizes_[number_in_vector_]];
	bool found = false;

	while (!message_streams_[number_in_vector_]->atEnd() && !found) {
		// apply for the whole file
		if (message_streams_[number_in_vector_]->readRawData(skype_header, 4) != 4) {
			// TODO(ashl1future): report error
			return false;
		}
		if (qstrncmp(skype_header, main_header_char_, kMainHeaderLength) != 0) {
			// TODO(ashl1future): report error
			return false;
		}
		if (message_streams_[number_in_vector_]->skipRawData(4) != 4) {
			// TODO(ashl1future): report error
			return false;
		}
		message_streams_[number_in_vector_]->readRawData(skype_block_char, messages_block_sizes_[number_in_vector_]);
		skype_block = QByteArray::fromRawData(skype_block_char, messages_block_sizes_[number_in_vector_]);

		block_type = skype_block[5];
		// skip not normal blocks
		// 0x01 - Dummy? Filled just only first 16 byte
		// 0x0a - Add user to chat?
		// 0x0b - see 0x0c
		// 0x0c - message synchronization between clients? Debug info?
		switch(skype_block[5]) {
		case 0x01:
			continue;
		case 0x0A:
		case 0x0B:
		case 0x0C:
			continue;
		}

		strpos = 0;
		strpos = skype_block.indexOf(chatmsg_file_chat_id_header_, strpos);
		if (strpos < 0) {
			// TODO(ashl1future): report error
			qDebug() << "Cannot found chat id";
			return false;
		}
		// TODO(ashl1future): report error
		ReadUtf8StringOrDie(skype_block, strpos + kChatmsgFileChatIdHeaderLength, chat_id);

		strpos = skype_block.indexOf(date_header_, strpos);
		if (strpos < 0) {
			// TODO(ashl1future): report error
			qDebug() << "Cannot found date header";
			return false;
		}
		// TODO(ashl1future): report error
		ReadDateTime(skype_block, strpos + kDateHeaderLength, time);
		if (time >= time_from) {
			strpos = skype_block.indexOf(sender_login_header_, strpos);
			if (strpos < 0) {
				// TODO(ashl1future): report error
				return false;
			}
			// TODO(ashl1future): report error
			ReadUtf8StringOrDie(skype_block, strpos + kSenderLoginHeaderLength, sender_login);
			//qDebug() << "From""" << sender_login << """at" << time.toString() << "in chat" << chat_id << ":";

			strpos = skype_block.indexOf(body_header_, strpos);
			if (strpos < 0) {
				// TODO(ashl1future): report error
				qDebug() << "cannot found body header" << (quint8)block_type;
				qDebug() << skype_block.toHex();
				// we should continue because we don't filter correct blocks with no body message
				continue;
				return false;
			}
			// TODO(ashl1future): report error
			ReadUtf8StringOrDie(skype_block, strpos + kBodyHeaderLength, body);
			//qDebug() << body;

			PluginChat* chat = NULL;
			PluginAccount* account = NULL;
			// TODO(ashl1future): report error
			GetChatOrDie(chat_id, chat);
			// TODO(ashl1future): report error
			GetAccountOrDie(sender_login, account);
			messages_[number_in_vector_] = new PluginMessage(account, body, chat, client(), time);

			messages_dates_[number_in_vector_] = time;
			messages_body_sizes_[number_in_vector_] = body.size();
			found = true;
		}
	}
	delete[] skype_block_char;

	if (!found) {
		messages_[number_in_vector_] = NULL;
		messages_dates_[number_in_vector_] = QDateTime();
		messages_body_sizes_[number_in_vector_] = 0;
	}

	return true;
}

PluginProtocol* PluginSkypeLinux::protocol() {
	if (!protocol_)
		protocol_ = new PluginProtocol("Skype");
	return protocol_;
}

void PluginSkypeLinux::UninitMessagesStructures() {
	// TODO(ashl1future): think how we clean messages_. It's not simple because the pointers may
	// be in messages_to_export_
	for (QList<PluginMessage*>::iterator messages_iterator = messages_.begin();
			messages_iterator != messages_.end(); ++messages_iterator) {
		if (messages_to_export_.indexOf(*messages_iterator) < 0)
			delete *messages_iterator;
	}
	messages_.clear();

	messages_dates_.clear();
	messages_absolute_filenames_.clear();
	messages_block_sizes_.clear();
	messages_body_sizes_.clear();

	QList<QDataStream*>::iterator stream_iterator = message_streams_.begin();
	QList<QFile*>::iterator file_iterator = messages_files_.begin();
	for (; stream_iterator != message_streams_.end(); ++stream_iterator, ++file_iterator) {
		delete *stream_iterator;
		delete *file_iterator;
	}
	message_streams_.clear();
	messages_files_.clear();
}

Q_EXPORT_PLUGIN2(impi-skype-linux, PluginSkypeLinux)
