#include<bits/stdc++.h>
#include<fstream>

using namespace std;

class DocumentElement {
    public:
        virtual string render() = 0;
};

class TextElement : public DocumentElement {
    private:
        string text;
    public:
        TextElement(string text){
            this->text = text;
        }
        string render() override {
            return text;
        }
};

class ImageElement: public DocumentElement {
    private:
        string imagePath;
    public:
        ImageElement(string imagePath){
            this->imagePath = imagePath;
        }
        string render() override {
            return "[Image: "+ imagePath + "]";
        }
};

class NewLineElement: public DocumentElement {
    public:
        string render() override{
            return "\n";
        }
};

class TabSpaceElement: public DocumentElement {
    public:
        string render() override {
            return "\t";
        }
};

class Document {
    private:
        vector<DocumentElement*> documentElements;
    public:
        void addElement(DocumentElement* element) {
            documentElements.push_back(element);
        }
        string render() {
            string result = "";
            for(auto ele: documentElements){
                result+= ele->render();
            }
            return result;
        }
};

class Persistence {
    public:
        virtual void save(string data) = 0;
};

class FileStorage: public Persistence {
    public:
        void save(string data) override {
            ofstream outFile("document.txt");
            if(outFile){
                outFile<<data;
                outFile.close();
                cout<<" Document saved to document.txt"<<endl;
            }
            else{
                cout<<"Error: unable to open for writring"<<endl;
            }
        }
};

class DBStorage: public Persistence{
    public:
        void save(string data) override {
            //save to DB
        }
};

class DocumentEditor {
    private:
        Document* document;
        Persistence* storage;
        string renderDocument;

    public:
        DocumentEditor(Document* document, Persistence* storage){
            this->document = document;
            this->storage = storage;
        }
        void addtext(string text){
            document->addElement(new TextElement(text));
        }
        void addImage(string imagePath){
            document->addElement(new ImageElement(imagePath));
        }
        void addnewLine(){
            document->addElement(new NewLineElement());
        }
        void addTabspace(){
            document->addElement(new TabSpaceElement());
        }

        string RenderDocument(){
            if(renderDocument.empty()){
                renderDocument = document->render();
            }
            return renderDocument;
        }
        void saveDocument(){
            storage->save(RenderDocument());
        }
};

int main(){
    Document* document = new Document();
    Persistence* persistence = new FileStorage();
    DocumentEditor* editor = new DocumentEditor(document, persistence);

    editor->addtext("Hello, this is sample how google doc workd.");
    editor->addnewLine();
    editor->addtext("carefully go thorugh every part of code");
    editor->addnewLine();
    editor->addTabspace();
    editor->addtext("Indented text");
    editor->addImage("shreyas.jpg");

    cout<<editor->RenderDocument()<<endl;
    editor->saveDocument();

    return 0;
}