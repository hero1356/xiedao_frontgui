#include "ShowTravlerScoreDlg.h"
#include "ui_ShowTravlerScoreDlg.h"

ShowTravlerScoreDlg::ShowTravlerScoreDlg(QString strResult, QString current_data, QWidget *parent) : QDialog(parent,Qt::WindowCloseButtonHint),
    ui(new Ui::ShowTravlerScoreDlg)
{
    ui->setupUi(this);

    setFixedSize(size());

    qDebug() << "ShowTravlerScoreDlg : strResult:"<< strResult;
    // json 解析
    score_info_st score_info;
    QString errmsg;
    bool ret = travelerScoreParse(strResult, score_info, errmsg);
    if(ret)
    {
        m_name = score_info.name;
        if(score_info.score_class == "NOSCORE")
        {
            score_info.score_class = ": 无成绩";
        }
        if(score_info.suggestion == "NOSCORE")
        {
            score_info.suggestion = "一起学习吧！";
        }
//        QString current_date =QDateTime::currentDateTime().toString("yyyy年MM月dd日");
        paiban(score_info, current_data);
    }

}


//成绩json解析

void ShowTravlerScoreDlg::paiban(score_info_st score_info, QString current_data)
{
    QFont font_title(QFont("隶书", 30));
    QFont font_name(QFont("隶书", 20));
    QFont font_grade(QFont("隶书", 20));
    QFont font_items(QFont("楷体", 12));
    QFont font_addition(QFont("隶书", 20));
    QFont font_encourage(QFont("隶书", 20));

    QString p_name =  QString("%1同学：").arg(score_info.name);
    QString p_score = QString("    恭喜您于%1完成了公共安全体验，总分：%2分，成绩%3，%4").arg(current_data).arg(score_info.finalscore).arg(score_info.score_class).arg(score_info.suggestion);
    QString p_items = QString("    您此次未体验的项目有：");

    if(!score_info.remain_list.isEmpty())
    {
        for(int i=0; i<score_info.remain_list.count()-1; i++)
        {
            p_items.append(score_info.remain_list.at(i));
            p_items.append("、");
        }
        p_items.append(score_info.remain_list.at(score_info.remain_list.count()-1));
        p_items.append(QString("！"));
        p_items.append(QString("欢迎您下次再来！"));
    }
    QString p_addition = QString("您的成绩优异，荣获公共安全明星奖！");
    QString p_encourage = QString("特发此证，以兹鼓励！");

    //开始排版

//    QTextEdit* edit = new QTextEdit;

    QTextDocument* doc = ui->edit->document();
    QTextCursor cursor = ui->edit->textCursor();
    QTextFrame *root_frame = doc->rootFrame();
    QTextFrameFormat root_frame_format = root_frame->frameFormat();//创建框架格式
    root_frame_format.setMargin(250);
    root_frame->setFrameFormat(root_frame_format); //给框架使用格式

    QTextBlockFormat textBlockFormat;
    textBlockFormat.setTopMargin(15);//设置段前行距：15
    textBlockFormat.setBottomMargin(15);//设置段后行距：15
    textBlockFormat.setLeftMargin(20);//设置段左行距：20
    textBlockFormat.setRightMargin(20);//设置段右行距：20
    textBlockFormat.setLineHeight(100,QTextBlockFormat::ProportionalHeight);//设置行间距，单倍行距

    // 显示标题段落
    {
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_title);
        //段落格式

        textBlockFormat.setAlignment(Qt::AlignHCenter);//对其方式：居中对其
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText("   \n",char_format);
    }

    // 显示姓名段落
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_name);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);//对其方式：两端对齐
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_name,char_format);
    }

    // 显示成绩段落
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_grade);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);//对其方式：居中对齐
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_score,char_format);
    }

    // 显示未体验项目段落
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_items);

        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignJustify);//对其方式：两端对齐
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_items,char_format);
    }

    // 若成绩大于90分，则显示附加段落段落
    {
       //int score = score_info.finalscore.toDouble();
       //if(score >= 70)
        QString score_class = score_info.score_class;
        if(score_class == "优秀")
       {
           cursor.insertBlock();
           // 字体格式
           QTextCharFormat char_format;
           char_format.setFont(font_addition);
           QColor color("red");
           char_format.setForeground(color);

           // 段落格式
           textBlockFormat.setAlignment(Qt::AlignRight);//对其方式：右对齐
           cursor.setBlockFormat(textBlockFormat);
           // 插入内容
           cursor.insertText(p_addition,char_format);
       }
    }

    // 显示鼓励段落
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_encourage);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignRight);//对其方式：右对齐
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_encourage,char_format);
    }
}

void ShowTravlerScoreDlg::on_m_printBtn_clicked()
{
    // 调用本地打印机打印
    qDebug() << "==================  开始打印 =============";
    QPrintDialog dlg(this);
    dlg.setWindowTitle("打印");
    if(dlg.exec() == QPrintDialog::Accepted )
    {
        QPrinter* p = dlg.printer();
        p->setOrientation(QPrinter::Landscape);   //设置为横向
        p->setPaperSize(QPrinter::B5);            //设置纸张为B5
        QTextDocument *td = ui->edit->document();

        QString current_date =QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        QString path = QString("D:\\ScorePrint\\%1 - %2.pdf").arg(current_date).arg(m_name);
//        p->setOutputFileName(path);

        td->print(p);
    }
    qDebug() << "==================  打印成功 =============";
}


bool ShowTravlerScoreDlg::travelerScoreParse(QString Json, score_info_st& score_info, QString& errmsg)
{
    qDebug() << "Json_Parse begin...";

    bool success = false;
    errmsg = "Json_Parse no error";

//    int data_top,data_bottom;
//    QString framehead= "$";
//    QString frametail= "*";

//    data_top=Json.indexOf(framehead,0);
//    data_bottom=Json.indexOf(frametail,0);

//    QString rec_data=Json.mid((data_top),(data_bottom));//去掉"*AA\r\n"字段
//    //qDebug() << "receive  data: " << rec_data;
//    printf("----------------------");


//    int len = rec_data.indexOf("{");
//    QString buf=rec_data.right(rec_data.length()-len);

    QString buf = Json;


    QByteArray data = buf.toUtf8();
    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
    if (jsonError.error == QJsonParseError::NoError)
    {

        if (json.isObject())
        {
            QJsonObject rootObj = json.object();

            //是否含有key  type
            if(rootObj.contains("type"))
            {
                QJsonValue value = rootObj.value("type");
                //判断是否是string类型
                if (value.isString()){
                    score_info.type = value.toString();
                }else{
                    errmsg = "type is not string!";
                }
            }else{
                errmsg = "type is not contains";
            }

            //是否含有key  code
            if(rootObj.contains("code"))
            {
                QJsonValue value = rootObj.value("code");
                //判断是否是string类型
                if (value.isString()){
                    score_info.code = value.toString();
                }else{
                    errmsg = "code is not string!";
                }
            }else{
                errmsg = "code is not contained!";
            }

            //是否含有key  cardid
            if(rootObj.contains("cardid"))
            {
                QJsonValue value = rootObj.value("cardid");
                //判断是否是string类型
                if (value.isString()){
                    score_info.cardid = value.toString();
                }else{
                    errmsg = "cardid is not string!";
                }
            }else{
                errmsg = "cardid is not contained!";
            }

            //是否含有key  comid
            if(rootObj.contains("comid"))
            {
                QJsonValue value = rootObj.value("comid");
                //判断是否是string类型
                if (value.isString()){
                    score_info.comid = value.toString();
                }else{
                    errmsg = "comid is not string!";
                }
            }else{
                errmsg = "comid is not contained!";
            }



            //是否含有key  name
            if(rootObj.contains("name"))
            {
                QJsonValue value = rootObj.value("name");
                //判断是否是string类型
                if (value.isString()){
                    score_info.name = value.toString();
                    success = true;
                }else{
                    errmsg = "name is not string!";
                }
            }else{
                errmsg = "name is not contained!";
            }

            //是否含有key  finalscore
            if(rootObj.contains("finalscore"))
            {
                QJsonValue value = rootObj.value("finalscore");
                //判断是否是string类型
                if (value.isString()){
                    score_info.finalscore = value.toString();
                }else{
                    errmsg = "finalscore is not string!";
                }
            }else{
                errmsg = "finalscore is not contained!";
            }

            //是否含有key  credit_point
            if(rootObj.contains("credit_point"))
            {
                QJsonValue value = rootObj.value("credit_point");
                //判断是否是string类型
                if (value.isString()){
                    score_info.credit_point = value.toString();
                }else{
                    errmsg = "credit_point is not string!";
                }
            }else{
                errmsg = "credit_point is not contained!";
            }

            //是否含有key  grade_point
            if(rootObj.contains("grade_point"))
            {
                QJsonValue value = rootObj.value("grade_point");
                //判断是否是string类型
                if (value.isString()){
                    score_info.grade_point = value.toString();
                }else{
                    errmsg = "grade_point is not string!";
                }
            }else{
                errmsg = "grade_point is not contained!";
            }

            //是否含有key  class
            if(rootObj.contains("class"))
            {
                QJsonValue value = rootObj.value("class");
                //判断是否是string类型
                if (value.isString()){
                    score_info.score_class = value.toString();
                }else{
                    errmsg = "class is not string!";
                }
            }else{
                errmsg = "class is not contained!";
            }

            //是否含有key  suggestion
            if(rootObj.contains("suggestion"))
            {
                QJsonValue value = rootObj.value("suggestion");
                //判断是否是string类型
                if (value.isString()){
                    score_info.suggestion = value.toString();
                }else{
                    errmsg = "suggestion is not string!";
                }
            }else{
                errmsg = "suggestion is not contained!";
            }



            if(rootObj.contains("remainsobj"))
            {
                QJsonValue value = rootObj.value("remainsobj");
                if(value.isObject())
                {
                    QJsonObject obj = value.toObject();

                    //是否含有key  remains
                    if(obj.contains("remains"))
                    {
                        QJsonValue value = obj.value("remains");
                        //判断是否是string类型
                        if(value.isArray())
                        {
                            QJsonArray array = value.toArray();
                            int size = array.size();
                            score_info.remain_list.clear();
                            for(int i=0; i<size; i++)
                            {
                                QJsonValue arrayVal = array.at(i);
                                if(arrayVal.isNull())
                                {
                                    errmsg = "arrayVal is not null!";
                                }
                                if(arrayVal.isObject())
                                {
                                    QJsonObject arrayObj = arrayVal.toObject();
                                    if(arrayObj.contains("name"))
                                    {
                                        QJsonValue remainsVal = arrayObj.value("name");
                                        if(remainsVal.isString())
                                        {
                                            score_info.remain_list.append(remainsVal.toString());
                                            success = true;
                                        }else{
                                             errmsg = "remainsVal is not string";
                                        }
                                    }else{
                                        errmsg = "remainsObj name not contains!";
                                    }
                                }else{
                                    errmsg = "arrayVal is not object";
                                }
                            }

                        }else{
                            errmsg = "remains is not array";
                        }

                    }else{
                        errmsg = "remains is not contained!";
                    }

                }else{
                    errmsg = "remainsobj is not object";
                }
            }else{
                errmsg = "remainsobj is not contained";
            }




        }else{
            errmsg = "json is not object";
        }

    }else{
        errmsg = "json is error!";
    }

    qDebug() << errmsg;
    qDebug() << "Json_parse end...";
    return success;
}


ShowTravlerScoreDlg::~ShowTravlerScoreDlg()
{
    delete ui;
}

