#ifndef SIMPLEPLAGIARISMDETECTOR_H
#define SIMPLEPLAGIARISMDETECTOR_H

#include <QMainWindow>
#include "Includes.h"
#include "Document.h"
#include "BruteForceMatcher.h"
#include "Match.h"
#include "RabinKarp.h"
#include <QFile>
#include <QString>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class SimplePlagiarismDetector; }
QT_END_NAMESPACE

class SimplePlagiarismDetector : public QMainWindow
{
    Q_OBJECT

public:
    SimplePlagiarismDetector(QWidget *parent = nullptr);
    ~SimplePlagiarismDetector();

    void RabinKarpFunc(Document d1, Document d2);
    void BruteForce(Document d1, Document d2);
    float GetHammingDistances(vector<string> s1, vector<string> s2);

private slots:
    void on_start_clicked();

    void on_reset_clicked();

private:
    Ui::SimplePlagiarismDetector *ui;

    vector<Document> corpus;
    //Document Plagiarized;;
};
#endif // SIMPLEPLAGIARISMDETECTOR_H
