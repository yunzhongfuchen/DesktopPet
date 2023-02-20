#include "../../lib/stackWidget/stack_localMusic.h"
#include <QFile>


localMusic::localMusic(QWidget *parent):
    QWidget(parent)
{
    this->setMinimumSize(800,530);
    this->setObjectName("localmusic");

    local_VBoxLayout = new QVBoxLayout();
    local_VBoxLayout->setContentsMargins(0,0,0,0);
    this->setLayout(local_VBoxLayout);
    this->setAttribute(Qt::WA_StyledBackground);

    localMusicLayout();

    LoadStyleSheet();
    //Ҫ�ȼ�������ʽ�ټ����ӿؼ�����Ϊ��������Widget�������͸������õ�ͬһ��QSS��ʽ��
    //�ӿؼ����Զ��̳и��ؼ�����ʽ�����ǵø��ؼ��ȼ�������ʽ���У�����
    TabWidgetLoad();

    ConnectInit();
    DataBaseInit();

    /*Dir_list.append("�ҵ�����");
    QDir dir(QCoreApplication::applicationDirPath() + "/mp3");
    QDir dirbsolutePath(dir.absolutePath());
    Dir_list.append(dirbsolutePath.path());*/
}

void localMusic::LoadStyleSheet(void)
{
    QFile file(":/qss/stackqss/stack_localMusic.qss");
    file.open(QIODevice::ReadOnly);

    if (file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }
}

void localMusic::DataBaseInit(void)
{
    QString sql = "create table if not exists selectPath"
                  "(path STRING NOT NULL);";

    dbManager::excute(sql,"selectPath");

    QSqlQuery *query = nullptr;
    sql = QString("select * from selectPath;");
    query = dbManager::select(sql, "selectPath");

    if(query->first())
    {
        while (query->next())
        {
            //qDebug() << query->record().indexOf("path");
            QSqlRecord record = query->record();
            int FieldNo = record.indexOf("path");
            qDebug() << record.value(FieldNo).toString();

            Dir_list.append(record.value(FieldNo).toString());
        }
    }
    else
    {
        Dir_list.append(__QString("�ҵ�����"));
        QDir dir(QCoreApplication::applicationDirPath() + "/mp3");
        QDir dirbsolutePath(dir.absolutePath());
        Dir_list.append(dirbsolutePath.path());

        // ODBC����������벽�� 1Ԥ�������ռλ�� 2�ÿɱ��б��ռλ����ֵ 3��Ӱ�  4ִ��Ԥ����
        //Ԥ�������ռλ��
        sql = "insert into selectPath (path) values(?);";
        query = dbManager::select(sql, "selectPath");

        QStringList pathlist = this->Dir_list;

        query->addBindValue(pathlist);
        query->execBatch();

        //sql = "insert into selectPath (path) values(:path);";

        //query = dbManager::select(sql, "selectPath");

        //for (int i=0; i<Dir_list.count(); i++)
        //{
            //query->bindValue(":path",Dir_list.at(i));
            //query->bindValue(0,Dir_list.at(i));
            //query->exec();
        //}
    }

    delete query;
}

void localMusic::GetplayPath(void)
{
   Dir_list.clear();

    QString sql = "select * from selectPath;";

    QSqlQuery *query = nullptr;

    query = dbManager::select(sql, "selectPath");

    if (query->first())
    {
        do
        {
            Dir_list.append(query->value(0).toString());
        }
        while (query->next());
    }
    delete query;
}

void localMusic::SaveplayPath(void)
{
    QString sql;

    for (int i=0; i<Dir_list.count(); i++)
    {
        QSqlQuery *query = nullptr;
        sql = QString("select * from selectPath where path = '%1';").arg(Dir_list.at(i));
        query = dbManager::select(sql, "selectPath");
        if (!query->first())    //��ѯ���ݿ����Ƿ���ڸ�·�����������ڣ���������ݿ���
        {
            sql = QString("insert into selectPath (path) values('%1');").arg(Dir_list.at(i));
            dbManager::excute(sql, "selectPath");
        }
        delete query;
    }
}

void localMusic::localMusicLayout(void)
{
    display_HBoxLayout = new QHBoxLayout();

    displayLabel[0] = new QLabel();
    displayLabel[0]->setText(__QString("��������"));
    displayLabel[0]->setObjectName("localLabel");
    displayLabel[1] = new QLabel();
    displayLabel[1]->setText(__QString("��0��"));
    displayLabel[1]->setObjectName("numberlabel");

    selectDir = new QPushButton();
    selectDir->setText(__QString("ѡ��Ŀ¼"));
    selectDir->setObjectName("selectButton");
    selectDir->setCursor(Qt::PointingHandCursor);

    QSpacerItem *display_hSpacer = new QSpacerItem(200,10,
                                                   QSizePolicy::Expanding,
                                                   QSizePolicy::Expanding);

    display_HBoxLayout->addWidget(displayLabel[0]);
    display_HBoxLayout->addWidget(displayLabel[1]);
    display_HBoxLayout->addSpacerItem(display_hSpacer);
    display_HBoxLayout->addWidget(selectDir);
    display_HBoxLayout->addSpacing(30);
}

void localMusic::TabWidgetLoad(void)
{
    QWidget *singer = new QWidget();
    QWidget *album = new QWidget();
    QWidget *folder = new QWidget();

    //����Tab�����ʼ��
    songInfoLayout();


    //������ϢTabҳ
    musicInfo_Widget = new QTabWidget();
    musicInfo_Widget->setObjectName("musicIndo_TabWidget");
    musicInfo_Widget->setMinimumSize(800,400);
    musicInfo_Widget->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);
    musicInfo_Widget->tabBar()->setObjectName("musicInfo_TabBar");
    musicInfo_Widget->addTab(song, __QString("����"));
    musicInfo_Widget->addTab(singer, __QString("����"));
    musicInfo_Widget->addTab(album, __QString("ר��"));
    musicInfo_Widget->addTab(folder, __QString("�ļ���"));

    local_VBoxLayout->addSpacing(20);
    local_VBoxLayout->addLayout(display_HBoxLayout);
    local_VBoxLayout->addWidget(musicInfo_Widget);
}

void localMusic::songInfoLayout(void)
{
    //-----------------��ť��ʼ��---------------------//
    song = new QWidget();
    QVBoxLayout *song_VLayout = new QVBoxLayout();

    playAll = new QPushButton();
    playAll->setText(__QString("����ȫ��"));
    playAll->setMaximumSize(120,35);
    playAll->setMinimumSize(120,35);
    playAll->setObjectName("playallBtn");
    playAll->setIcon(QIcon(":/images/stackWidget/localMusic/btn_playall.png"));

    /*QAction *addAllAction = new QAction(playAll);
    addAllAction->setIcon(QIcon(":/images/stackWidget/localMusic/btn_playalladd.png"));
    playAll->addAction(addAllAction);*/

    matchMusic = new QPushButton();
    matchMusic->setText(__QString("����б�"));
    matchMusic->setIcon(QIcon(QPixmap(":/images/stackWidget/localMusic/btn_deleteAll.png")));
    matchMusic->setObjectName("matchMusicbtn");
    matchMusic->setMaximumSize(120,35);
    matchMusic->setMinimumSize(120,35);
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText(__QString("������������"));
    searchEdit->setObjectName("localSearch");
    searchEdit->setMaximumSize(200,25);

    QAction *searchAction = new QAction(searchEdit);
    searchAction->setIcon(QIcon(":/images/stackWidget/localMusic/icon_search.png"));
    //����ICON���������ұ�
    searchEdit->addAction(searchAction,QLineEdit::TrailingPosition);

    QSpacerItem *Button_HSpacer = new QSpacerItem(200,20,
                                                  QSizePolicy::Expanding,
                                                  QSizePolicy::Expanding);


    QHBoxLayout *button_HBoxLayout = new QHBoxLayout();
    button_HBoxLayout->addWidget(playAll);
    button_HBoxLayout->addWidget(matchMusic);
    button_HBoxLayout->addSpacerItem(Button_HSpacer);
    button_HBoxLayout->addWidget(searchEdit);

    music_Table = new QTableWidget();
    music_Table->setMinimumSize(750,500);
    music_Table->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::Expanding);
    music_Table->setObjectName("table_music");
    QList <QString> tableList;
    tableList = __QString(",���ֱ���,����,ר��,ʱ��,��С").split(",");
    music_Table->setColumnCount(6);
    music_Table->setHorizontalHeaderLabels(tableList);
    music_Table->setColumnWidth(0,10);
    music_Table->setColumnWidth(1,200);
    music_Table->setColumnWidth(2,100);
    music_Table->setColumnWidth(3,250);
    music_Table->setColumnWidth(4,70);
    music_Table->setColumnWidth(5,70);
    //���Ҽ��˵�����
    music_Table->setContextMenuPolicy(Qt::CustomContextMenu);
    //���������б���ɫ
    music_Table->setAlternatingRowColors(true);
    //���ñ�����ݲ��ɱ༭
    music_Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //����Ϊ��ѡ��
    music_Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //�������һ������Ӧ����
    music_Table->horizontalHeader()->setStretchLastSection(true);
    //ɾ�������
    music_Table->setShowGrid(false);
    //ȥ���߿���
    music_Table->setFrameShape(QFrame::NoFrame);
    //ȥ��ѡ�����߿�
    music_Table->setFocusPolicy(Qt::NoFocus);
    //���õ����ͷ��������
    music_Table->setSortingEnabled(true);
    //���ñ�ͷ����������
    music_Table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //���õ����Ԫ��ʱ����ͷ�����
    music_Table->horizontalHeader()->setHighlightSections(false);
    music_Table->verticalHeader()->setHighlightSections(false);
    //����ֻ��ѡ��һ��Ŀ��
    music_Table->setSelectionMode(QAbstractItemView::SingleSelection);
    //���ô�ֱ��������С���
    music_Table->verticalScrollBar()->setMaximumWidth(7);

    music_Table->verticalHeader()->setAlternatingRowColors(true);
    music_Table->verticalHeader()->setObjectName("music_verticalHeader");
    music_Table->verticalHeader()->setDefaultSectionSize(30);
    music_Table->verticalHeader()->setVisible(false);

    song_VLayout->addLayout(button_HBoxLayout);
    song_VLayout->addWidget(music_Table);

    song->setLayout(song_VLayout);
}

void localMusic::table_customContextMenuRequest(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    QAction *ViewComments = menu->addAction(__QString("�鿴����"));
    QAction *play = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_play.png"), __QString("����"));
    QAction *nextPlay = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_playnext.png"), __QString("��һ�ײ���"));
    menu->addSeparator();
    QAction *Collecttolist = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_collectlist.png"), __QString("�ղص��赥"));
    QAction *sharemusic = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_share.png"), __QString("����"));
    QAction *coppy = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_coppy.png"), __QString("��������"));
    QAction *uploadcloud = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_cloudmusic.png"), __QString("�ϴ����ҵ���������"));
    QAction *opendir = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_folder.png"), __QString("���ļ�����Ŀ¼"));
    menu->addSeparator();
    QAction *deletefromlist = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_delete.png"), __QString("���б���ɾ��(Delete)"));
    QAction *deletefromlocal = menu->addAction(QIcon(":/images/stackWidget/localMusic/btn_delete.png"), __QString("�ӱ��ش���ɾ��"));

    QMenu *collectChild = new QMenu(this);
    Collecttolist->setMenu(collectChild);

    QAction *creatnewSheet = collectChild->addAction(QIcon(":/images/stackWidget/localMusic/btn_add.png"), __QString("�����¸赥"));
    QAction *myfavor = collectChild->addAction(QIcon(":/images/stackWidget/localMusic/btn_add.png"), __QString("��ϲ��������"));


    //menu.addAction("",this,SLOT());

    menu->exec(QCursor::pos());

}

void localMusic::ConnectInit(void)
{
    //tablelist�����Ҽ������˵���
    connect(music_Table,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(table_customContextMenuRequest(QPoint)));
    //ѡ��Ŀ¼
    connect(selectDir,SIGNAL(clicked()),
            this,SLOT(btn_selectDirSlot()));

    //����ȫ����ť
    connect(playAll,SIGNAL(clicked()),
            this,SLOT(btn_addlistclicked_slot()));
    //����б�ť
    connect(matchMusic,SIGNAL(clicked()),
            this,SLOT(btn_deleteclicked_slot()));

    /*connect(playAll,SIGNAL(clicked()),
            playlist,SLOT(Rev_ListMsg_slot(QList <MediaObjectInfo1> *info)));*/

}

void localMusic::btn_deleteclicked_slot(void)
{
    mediaObjectInfo.clear();
    music_Table->clearContents();
    music_Table->setRowCount(0);

	displayLabel[1]->setText(__QString("��") + QString::number(mediaObjectInfo.count(), 10) + __QString("��"));
}

void localMusic::btn_addlistclicked_slot(void)
{
    emit addall_signal(&this->mediaObjectInfo);
}

void localMusic::btn_selectDirSlot(void)
{
    //��ȡ����λ��x,y
    int x = this->parentWidget()->parentWidget()->parentWidget()->mapToGlobal(QPoint(0,0)).rx() +
            parentWidget()->parentWidget()->parentWidget()->width()/2 -
            450/2;
    int y = this->parentWidget()->parentWidget()->parentWidget()->mapToGlobal(QPoint(0,0)).ry() +
            parentWidget()->parentWidget()->parentWidget()->height()/2 -
            300/2;
    GetplayPath();

    //selectDialog *selectDialog = new class selectDialog(this);
    selectDialog *selectDialog = new class selectDialog(this,Dir_list);
    selectDialog->move(x,y);
    //���ùرմ���ʱ�Զ�����
    //selectDialog->setAttribute(Qt::WA_DeleteOnClose);

    //��ģ̬��ʽ��ʾ�Ի���
    int ret = selectDialog->exec();

    int count = 0;

    if(ret == QDialog::Accepted)
    {
        Dir_list = selectDialog->Dir_List;

        SaveplayPath();
        foreach (QString str, selectDialog->Dir_List)
        {
            //�����Ŀ¼��ѡ��
            if(selectDialog->listPath->item(count)->checkState() == Qt::Checked)
            {
                QDir dir(str);
                if (dir.exists())
                {
                    //���������
                    QStringList filter;
                    //��������.mp3��׺���ļ�
                    filter << "*.mp3";
                    //��ȡ��Ŀ¼�µ������ļ�
                    QFileInfoList files = dir.entryInfoList(filter,QDir::Files);

                    for(int i=0; i< files.count(); i++)
                    {
                        QString songname = QString::fromUtf8(files.at(i).fileName().replace(".mp3","").toUtf8().data());

                        bool result = false;
                        foreach(MediaObjectInfo search,mediaObjectInfo)
                        {
                            if(search.filePath == files.at(i).filePath())
                            {
                                result = true;
                            }
                        }

                        if (result == false)
                        {
                            MediaObjectInfo info;
                            //qDebug() << files.at(i).fileName();

                            music_Table->insertRow(music_Table->rowCount());
                            int index = music_Table->rowCount()-1;
                            if(index < 0)
                                index =0;

                            QString number;
                            if (index+1<10)
                                number = "0" + QString::number(index+1,10);
                            else
                                number = QString::number(index+1,10);

                            QTableWidgetItem *item0 = new QTableWidgetItem(number);
                            music_Table->setItem(index,0,item0);

                            QStringList songinfo = songname.split("-");

                            info.fileName = songinfo.at(0);

                            if (songinfo.count()>1)
                                info.singer = songinfo.at(1);
                            else
                                info.singer = "δ֪����";

                            info.playtime = "00:00";

                            info.filePath = files.at(i).filePath();
                            //qDebug() << "path:" << files.at(i).filePath();

                            QTableWidgetItem *item1 = new QTableWidgetItem(info.fileName);
                            music_Table->setItem(index,1,item1);
                            QTableWidgetItem *item2 = new QTableWidgetItem(info.singer);
                            music_Table->setItem(index,2,item2);
                            QTableWidgetItem* item3 = new QTableWidgetItem("δ֪ר��");
                            music_Table->setItem(index,3,item3);
                            QTableWidgetItem* item4 = new QTableWidgetItem(info.playtime);
                            music_Table->setItem(index,4,item4);

                            float size = (float)files.at(i).size()/1000.0f/1000.0f;
                            QString string_size = QString::number(size,'f',1) + "M";

                            QTableWidgetItem* item5 = new QTableWidgetItem(string_size);
                            music_Table->setItem(index,5,item5);

                            info.filesize = string_size;
                            mediaObjectInfo.append(info);
                        }
                    }
                }
            }
            count++;
        }
        music_Table->setRowCount(mediaObjectInfo.count());
        displayLabel[1]->setText(__QString("��")+QString::number(mediaObjectInfo.count(),10)+ __QString("��"));
    }

    delete selectDialog;
}






//---------------------------Dialog--------------------------------------//
selectDialog::selectDialog(QWidget *parent) :
    QDialog(parent)
{
    QDesktopWidget *desktopWidget = QApplication::desktop();

    int x = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->width()/2-225+desktopWidget->availableGeometry().x();
    int y = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->height()/2-150+desktopWidget->availableGeometry().y();

    this->setGeometry(x,y,450,300);
    //this->setWindowFlag(Qt::FramelessWindowHint);

    dialog_vLayout = new QVBoxLayout();

    select_LayoutInit();

    LoadStyleSheet();

    ConnectInit();
}

selectDialog::selectDialog(QWidget *parent ,QList <QString> list):
    QDialog(parent)
{
    Dir_List = list;

    QDesktopWidget *desktopWidget = QApplication::desktop();

    int x = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->width()/2-225+desktopWidget->availableGeometry().x();
    int y = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->height()/2-150+desktopWidget->availableGeometry().y();

    this->setGeometry(x,y,450,300);
    //this->setWindowFlag(Qt::FramelessWindowHint);

    dialog_vLayout = new QVBoxLayout();

    select_LayoutInit();

    LoadStyleSheet();

    ConnectInit();
}

void selectDialog::LoadStyleSheet(void)
{
    QFile file(":/qss/stackqss/stack_localMusicDialog.qss");

    file.open(QIODevice::ReadOnly);

    if (file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }
}

void selectDialog::select_LayoutInit(void)
{
    closeButton = new QPushButton();
    closeButton->setObjectName("selectDialog_closeButton");
    closeButton->setMaximumSize(20,20);
    closeButton->setCursor(Qt::PointingHandCursor);
    QHBoxLayout *Head_hLayout = new QHBoxLayout();
    Head_hLayout->addStretch();
    Head_hLayout->addWidget(closeButton);
    Head_hLayout->addSpacing(5);


    headLabel = new QLabel();
    headLabel->setMinimumSize(450,30);
    headLabel->setText(__QString("ѡ�񱾵������ļ���"));
    headLabel->setAlignment(Qt::AlignCenter);

    QFont font = headLabel->font();
    font.setBold(true);
    font.setPointSize(13);
    headLabel->setFont(font);

    describLabel = new QLabel();
    describLabel->setText(__QString("���Զ�ɨ���㹴ѡ��Ŀ¼���ļ�����ɾʵʱͬ����"));

    QPalette pe;
    pe.setColor(QPalette::WindowText,QColor(115,115,115));
    describLabel->setPalette(pe);

    /*Dir_List.append("�ҵ�����");
    QDir dir(QCoreApplication::applicationDirPath() + "/mp3");
    QDir dirbsolutePath(dir.absolutePath());
    Dir_List.append(dirbsolutePath.path());*/

    listPath = new QListWidget();
    listPath->setViewMode(QListView::ListMode);
    listPath->setObjectName("selectDialog_listPath");
    listPath->setMinimumSize(350,100);
    listPath->setFocusPolicy(Qt::NoFocus);
    listPath->verticalScrollBar()->setFixedWidth(7);

    foreach(QString path, Dir_List)
    {
        QListWidgetItem *pathItem = new QListWidgetItem();
        //����Item�߶�
        pathItem->setSizeHint(QSize(0,30));
        pathItem->setText(path);
        pathItem->setCheckState(Qt::Checked);
        listPath->addItem(pathItem);    //����һ����Ŀ
    }

    confirm_Button = new QPushButton();
    confirm_Button->setText(__QString("ȷ��"));
    confirm_Button->setMaximumSize(90,36);
    confirm_Button->setMinimumSize(90,36);
    confirm_Button->setObjectName("selectDialog_confirbtn");
    addfolder_Button = new QPushButton();
    addfolder_Button->setText(__QString("����ļ���"));
    addfolder_Button->setMaximumSize(115,36);
    addfolder_Button->setMinimumSize(115,36);
    addfolder_Button->setObjectName("selectDialog_addbtn");

    QHBoxLayout *button_hLayout = new QHBoxLayout();

    button_hLayout->addStretch();
    button_hLayout->addWidget(confirm_Button);
    button_hLayout->addSpacing(5);
    button_hLayout->addWidget(addfolder_Button);
    button_hLayout->addStretch();

    dialog_vLayout->addLayout(Head_hLayout);
    dialog_vLayout->addWidget(headLabel);
    dialog_vLayout->addSpacing(5);
    dialog_vLayout->addWidget(describLabel);
    dialog_vLayout->addSpacing(5);
    dialog_vLayout->addWidget(listPath);
    dialog_vLayout->addSpacing(5);
    dialog_vLayout->addLayout(button_hLayout);
    dialog_vLayout->addSpacing(20);

    this->setLayout(dialog_vLayout);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_StyledBackground);
    //�����ޱ߿�dialog����ק
    this->setSizeGripEnabled(true);
    //���ô��ڲ��ܸı��С
    this->setFixedSize(this->geometry().size());


    //��Dialog����Բ�Ǳ߿�
    QBitmap bmp(QSize(this->size()));
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),5,5);
    setMask(bmp);
}

void selectDialog::ConnectInit(void)
{
    //�رհ�ť��
    connect(closeButton,SIGNAL(clicked()),this,SLOT(btn_close_slot()));

    //ȷ����ť��
    connect(confirm_Button,SIGNAL(clicked()),this,SLOT(confirmclicked_slot()));
    //����ļ��а�ť��
    connect(addfolder_Button,SIGNAL(clicked()),this,SLOT(addfolder_slot()));
    //·��List�����
    connect(listPath,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(PathlistClicked_slot(QListWidgetItem *)));

}

void selectDialog::PathlistClicked_slot(QListWidgetItem *item)
{
    if(item->checkState()==Qt::Checked)
    {
        item->setCheckState(Qt::Unchecked);
    }
    else
    {
        item->setCheckState(Qt::Checked);
    }

}

void selectDialog::btn_close_slot(void)
{
    reject();
}

void selectDialog::confirmclicked_slot(void)
{
    accept();
}
void selectDialog::addfolder_slot(void)
{
    QString DirPath = QFileDialog::getExistingDirectory(this);
	if (DirPath == "")
	{
		return;
	}
    Dir_List.append(DirPath);

    QListWidgetItem *addItem = new QListWidgetItem();
    addItem->setText(DirPath);
    addItem->setCheckState(Qt::Checked);
    listPath->addItem(addItem);
    //qDebug() << "path:" << DirPath;
}

static QPoint dialog_last(0,0);        //��������

void selectDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->y() <100)
    {
        dialog_last = event->globalPos(); //��ȡ����ѹ��λ��
    }
}
void selectDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->y() <100)
    {
        int dx = event->globalX() - dialog_last.x();
        int dy = event->globalY() - dialog_last.y();
        dialog_last = event->globalPos();
        this->move(this->x()+dx,this->y()+dy);
    }
}
void selectDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->y() <100)
    {
        int dx = event->globalX() - dialog_last.x();
        int dy = event->globalY() - dialog_last.y();
        this->move(this->x()+dx,this->y()+dy);
    }
}
