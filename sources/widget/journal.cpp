#include "journal.h"

Journal::Journal(QWidget *parent) : QTextEdit(parent){
    this->setReadOnly(true);
    this->setLineWrapMode(QTextEdit::NoWrap);
    this->setUndoRedoEnabled(false);
}
void Journal::critical(int code, QString text){

}

void Journal::error(int code, QString text){

}

void Journal::warning(int code, QString text){

}

void Journal::info(int code, QString tex){

}
