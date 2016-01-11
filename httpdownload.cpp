#include "httpdownload.h"
#include "ui_httpdownload.h"
#include <QProcess>

HttpDownload::HttpDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HttpDownload)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    progressDialog = new QProgressDialog(this);
    setWindowIcon(QIcon(qApp->applicationDirPath()+"/src/icon.png"));

    ui->pushButton->setVisible(false);
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
}

HttpDownload::~HttpDownload()
{
    delete ui;
}

void HttpDownload::on_downloadButton_clicked()
{
    manager = new QNetworkAccessManager(this);

    // get url
    url = ("https://4f127c4feef351b26b75ad61883fdaec47fdc464.googledrive.com/host/0B8ftKunTL2iLeWo1NXE5LWJtLWM/bestsoft-spacelinux.zip");

    QFileInfo fileInfo(url.path());
    QString fileName = qApp->applicationDirPath()+"/"+fileInfo.fileName();
    QDir destination;
    destination.remove(qApp->applicationDirPath()+"/src");
    destination.remove(fileName);
    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("HTTP"),
                tr("There already exists a file called %1 in "
                "the current directory. Overwrite?").arg(fileName),
                QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                == QMessageBox::No)
                return;
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("HTTP"),
                      tr("Unable to save the file %1: %2.")
                      .arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }

    // used for progressDialog
    // This will be set true when canceled from progress dialog
    httpRequestAborted = false;

    progressDialog->setWindowTitle(tr("HTTP"));
    progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));

    // download button disabled after requesting download
    ui->downloadButton->setEnabled(false);

    startRequest(url);
}

void HttpDownload::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

void HttpDownload::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;

    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(bytesRead);
}

void HttpDownload::on_quitButton_clicked()
{
    this->close();
}

void HttpDownload::on_urlEdit_returnPressed()
{
    on_downloadButton_clicked();
}

void HttpDownload::enableDownloadButton()
{
    //ui->downloadButton->setEnabled(!(ui->urlEdit->text()).isEmpty());
}

// During the download progress, it can be canceled
void HttpDownload::cancelDownload()
{
    ui->statusLabel->setText(tr("Download canceled."));
    httpRequestAborted = true;
    reply->abort();
    ui->downloadButton->setEnabled(true);
}

// When download finished or canceled, this will be called
void HttpDownload::httpDownloadFinished()
{
    // when canceled
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        progressDialog->hide();
        return;
    }

    // download finished normally
    progressDialog->hide();
    file->flush();
    file->close();

    // get redirection url
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed: %1.")
                                 .arg(reply->errorString()));
        ui->downloadButton->setEnabled(true);
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url);
            return;
        }
    } else {
        QString fileName = QFileInfo(QUrl("D:/").path()).fileName();
        ui->statusLabel->setText(tr("Downloaded %1 to %2.").arg(fileName).arg(QDir::currentPath()));
        ui->downloadButton->setEnabled(true);
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
    manager = 0;
    QProcess *process = new QProcess();
    QStringList arguments;
    removeDir(qApp->applicationDirPath()+"/src");

    arguments << "-o"<< qApp->applicationDirPath()+"/bestsoft-spacelinux.zip"<<"-d"<<qApp->applicationDirPath();
    if (QFile::exists("/usr/bin/unzip")){
      process->start("/usr/bin/unzip",arguments);
      process->waitForFinished();
    }else{
        QMessageBox msgBox;
        msgBox.setText("To continue update you got to install unzip on your computer.");
        msgBox.exec();
    }
    ui->label->setText("Update Finished!");
    ui->pushButton->setVisible(true);

}
bool HttpDownload::removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

// This will be called when download button is clicked
void HttpDownload::startRequest(QUrl url)
{
    // get() method posts a request
    // to obtain the contents of the target request
    // and returns a new QNetworkReply object
    // opened for reading which emits
    // the readyRead() signal whenever new data arrives.
    reply = manager->get(QNetworkRequest(url));

    // Whenever more data is received from the network,
    // this readyRead() signal is emitted
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));

    // Also, downloadProgress() signal is emitted when data is received
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDownloadProgress(qint64,qint64)));

    // This signal is emitted when the reply has finished processing.
    // After this signal is emitted,
    // there will be no more updates to the reply's data or metadata.
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpDownloadFinished()));
}

void HttpDownload::on_pushButton_clicked()
{


    QProcess *p = new QProcess(this);
    QString filess = qApp->applicationDirPath()+"/Upgrd";
    p->startDetached(filess,QStringList()<<filess);
    qApp->quit();

    //delete p;
    //qApp->quit();
    //QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
