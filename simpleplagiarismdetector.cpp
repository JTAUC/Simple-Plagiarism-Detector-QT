#include "simpleplagiarismdetector.h"
#include "ui_simpleplagiarismdetector.h"

SimplePlagiarismDetector::SimplePlagiarismDetector(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SimplePlagiarismDetector)
{
    ui->setupUi(this);

    ui->TextBody->setVisible(false);
    ui->DocumentName->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->reset->setVisible(false);
}

SimplePlagiarismDetector::~SimplePlagiarismDetector()
{
    delete ui;
}

vector<Match> BF_exact_matches, RK_exact_matches;
BruteForceMatcher BFMatcher;
RabinKarp RabinKarpMatcher;

float BF_PlagiarizedCharCount = 0, RK_PlagiarizedCharCount = 0;

//Calculates hamming distance between each sentence in two documents,
//if it is less than a given constant it is considered plagiarism.
//Function returns number of plagiarized characters.

float SimplePlagiarismDetector::GetHammingDistances(vector<string> s1, vector<string> s2) {
    int commonCharacters = 0, c = 0;
    for (int i = 0; i < s1.size(); i++) {
        for (int j = 0; j < s2.size(); j++) {
            if (BFMatcher.HammingDistance(s1[i], s2[j], c) <= 5) {
                commonCharacters += c;
            }
        }
    }
    return commonCharacters;
}

void SimplePlagiarismDetector::BruteForce(Document d1, Document d2) {

    vector<string> d1_sentences = d1.getSentences(), d2_sentences = d2.getSentences();

    cout << "Hamming Distance: " << (GetHammingDistances(d1_sentences, d2_sentences) / d1.getFullText().length()) * 100 << endl;

    for (int i = 0; i < d2.getSentences().size(); i++) { //Sentence Matching
        int index = BFMatcher.runDetection(d1, d2_sentences[i]);
        if (index != -1) {
            BF_exact_matches.push_back(Match(i, index, d2_sentences[i], d1.getFileName()));
            BF_PlagiarizedCharCount += d2_sentences[i].length();
        }
    }
}

void SimplePlagiarismDetector::RabinKarpFunc(Document d1, Document d2) {
    vector<string> d1_sentences = d1.getSentences(), d2_sentences = d2.getSentences();
    int index = 0;

    for (int i = 0; i < d1.getSentences().size(); i++) { //Sentence Matching
        cout << d1_sentences[i] << endl;
        if (RabinKarpMatcher.runDetection(d1_sentences[i], d2.getFullText(), index)) {
            RK_exact_matches.push_back(Match(i, index, d1_sentences[i], d2.getFileName()));
            RK_PlagiarizedCharCount += d1_sentences[i].length();
        }
    }
}

void SimplePlagiarismDetector::on_start_clicked()
{
    QString s = ui->lineEdit->text();
    string path = s.toStdString();
    Document Plagiarized(path + "Plagiarized.txt");

    if (!Plagiarized.exists){
        cout << "DOC doesnt exist!" << endl;
        return;
    }
    if (!ui->checkBox->isChecked() && !ui->checkBox_2->isChecked()){
        return;
    }

    ui->TextBody->setVisible(true);
    ui->reset->setVisible(true);
    ui->DocumentName->setVisible(true);
    ui->progressBar->setVisible(true);
    ui->lineEdit->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->checkBox->setVisible(false);
    ui->start->setVisible(false);
    ui->textEdit->setVisible(false);

    corpus.clear();
    RK_exact_matches.clear();
    BF_exact_matches.clear();
    ui->TextBody->clear();
    RK_PlagiarizedCharCount = 0;
    BF_PlagiarizedCharCount = 0;
    float RK_plagiarismPercentage = 0;
    float BF_plagiarismPercentage = 0;
    float avgPlagiarism = 0;
    float totalCharCount = Plagiarized.getFullText().length();

    ui->DocumentName->setText(QString::fromStdString(Plagiarized.getFileName()));

    corpus.push_back(Document(path + "Doc1.txt"));
    corpus.push_back(Document(path + "Doc2.txt"));
    corpus.push_back(Document(path + "Doc3.txt"));
    corpus.push_back(Document(path + "Doc4.txt"));
    corpus.push_back(Document(path + "Doc5.txt"));

    vector<string> sentences = Plagiarized.getSentences();

    for (int i = 0; i < static_cast<int>(corpus.size()); i++) {
        if (ui->checkBox->isChecked())
            RabinKarpFunc(Plagiarized, corpus[i]);
        if (ui->checkBox_2->isChecked())
            BruteForce(corpus[i], Plagiarized);
    }

    RK_plagiarismPercentage = (RK_PlagiarizedCharCount / totalCharCount) * 100;
    BF_plagiarismPercentage = (BF_PlagiarizedCharCount / totalCharCount) * 100;

    avgPlagiarism = (RK_plagiarismPercentage + BF_plagiarismPercentage) / 2;

    cout << "Total Plagiarism Detected: " << avgPlagiarism << "%" << endl;

    ui->progressBar->setAlignment(Qt::AlignHCenter);
    ui->progressBar->setAlignment(Qt::AlignCenter);
    ui->progressBar->setValue(avgPlagiarism);

    RK_exact_matches.insert(RK_exact_matches.end(), BF_exact_matches.begin(), BF_exact_matches.end()); //Combine match vectors

    bool found = false;
    for (int i=0; i < static_cast<int>(sentences.size()); i++){
        for (int j=0; j < static_cast<int>(RK_exact_matches.size()); j++){
            if (i == RK_exact_matches[j].GetSentenceIndex()){
                found = true;
                ui->TextBody->setTextColor(Qt::red);
                ui->TextBody->insertPlainText(QString::fromStdString(sentences[i]));
            }
        }
        if (!found){
            ui->TextBody->setTextColor(Qt::black);
            ui->TextBody->insertPlainText(QString::fromStdString(sentences[i]));
        }
        found = false;
    }
    found = false;
}

void SimplePlagiarismDetector::on_reset_clicked()
{
    ui->TextBody->setVisible(false);
    ui->reset->setVisible(false);
    ui->DocumentName->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->lineEdit->setVisible(true);
    ui->checkBox_2->setVisible(true);
    ui->checkBox->setVisible(true);
    ui->start->setVisible(true);
    ui->textEdit->setVisible(true);
}
