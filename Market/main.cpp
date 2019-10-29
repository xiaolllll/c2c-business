#include "sell.h"
#include "dialog.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   //创建一个QApplication对象
    Dialog d;
    d.show();
    return a.exec();
//    Sell w;
//    /**
//    加入一个程序启动界面，但是有一点问题
//    */
//    QPixmap pixmap("Login.png");  //创建一个QPixmap对象,设置启动图片
//    QSplashScreen splash(pixmap);  //利用QPixmap对象创建一个QSplashScreen对象
//    splash.show();  //显示此启动图片
//    a.processEvents();  //使程序在显示启动画面的同时仍能响应鼠标其他事件
//    Dialog logIn;
//    splash.finish(&logIn);  //在主窗体对象初始化完成后,结束启动画面
//    if(w.logIn->exec()==QDialog::Accepted)// && w.connectServer())//连接
//    {
//        w.userInf = w.logIn->userInf;
//        w.getCommodityVector();
//        w.show();
//
//    }
//    else
//    {
//        QMessageBox::information(&w,"提示","请检查网络连接",QMessageBox::Ok);
//        return 0;
//    }
}
