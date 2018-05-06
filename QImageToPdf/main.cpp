#include <QtGui/QtGui>
#include <QtCore/QtCore>
#include <QtPrintSupport/QtPrintSupport>

void image_to_pdf(
        const QString & argImageFileDir,
        const QString & argImageName,
        const QString & argPdfDir,
        const QString & argPdfName
        ){
    const QDir varImageDir{ argImageFileDir };
    const QDir varPdfDir{ argPdfDir };
    QImage varImage{ varImageDir.absoluteFilePath( argImageName ) };
    double varImageWidth  = varImage.width();
    double varImageHeight = varImage.height();
    if( varImageWidth < 0.5 ){ return;  }
    if( varImageHeight < 0.5){ return;  }

    QPrinter varPrinter ; //
    varPrinter.setResolution( 300 );//300dpi
    varPrinter.setPageSize(QPrinter::A4); //设置纸张尺寸是A4
    varPrinter.setOutputFormat(QPrinter::PdfFormat); //设置输出格式pdf
    varPrinter.setPageOrientation(QPageLayout::Landscape);//设置纸张水平
    varPrinter.setFullPage(true);

    /* set to 1 : 1.414 */
    if( varImageHeight > varImageWidth ){
        const static QMatrix varMatrix =[](){
            QMatrix varMatrix;
            return varMatrix.rotate(  270. );
        }();
        varImage.transformed( varMatrix,Qt::SmoothTransformation );
        std::swap( varImageHeight , varImageWidth );
    }
    /* width >= height */
    const auto varP = varImageWidth / varImageWidth ;

    varPrinter.setOutputFileName( varPdfDir.absoluteFilePath(argPdfName) );
    class Painter : public QPainter {
     public:
        Painter( QPrinter * d){ this->begin(d); }
        ~Painter(){ this->end(); }
    } varPainter { &varPrinter } ;
    QRect varPainterViewPort = varPainter.viewport();

    QPointF varDrawPoition = varPainterViewPort.topLeft() ;
    if( varP > 1.4142135623731 ){
        varImage = varImage.scaledToWidth( varPainterViewPort.width(),Qt::SmoothTransformation );
        varDrawPoition.setY( std::abs(varPainterViewPort.height()-varImage.height())*0.5 );
    }else{
        varImage = varImage.scaledToHeight( varPainterViewPort.height(),Qt::SmoothTransformation );
        varDrawPoition.setX( std::abs(varPainterViewPort.width()-varImage.width())*0.5 );
    }
    varPainter.drawImage( varDrawPoition , varImage );

}


int main(int argc, char *argv[]){
    QCoreApplication app(argc, argv);

    //"E:\图片\E71001\71445\E71445.JPG"
    image_to_pdf(
                QString::fromUtf8(u8R"(E:\图片\E71001\71445)"),
                QString::fromUtf8(u8R"(E71445.JPG)"),
                QString::fromUtf8(u8R"(C:\Users\admin\Desktop\tmp)"),
                QString::fromUtf8(u8R"(test.pdf)")
                );


}
