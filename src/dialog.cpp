// Copyright (C) 2011-2012 pUML Group

#include <QtGui>

//#include <iostream>  // NOLINT

#include "./dialog.h"

//using std::cout;

/*! This constructor creates the widgets for this dialog and connects
    the Ok and Cancel buttons with the accept and reject slots. Uses
    createIcons, createNewPage and createOpenPage helper functions.
*/
ConfigDialog::ConfigDialog() {
  //contentsWidget is the sidebar navigation widget
  contentsWidget = new QListWidget;
  contentsWidget->setViewMode(QListView::IconMode);
  contentsWidget->setIconSize(QSize(96, 84));
  contentsWidget->setMovement(QListView::Static);
  contentsWidget->setMaximumWidth(128);
  contentsWidget->setSpacing(12);
  createIcons();


  //create our two pages
  newPage = new QWidget;
  openPage = new QWidget;
  createOpenPage();
  createNewPage();

  //add the pages to the stacked widget
  pagesWidget = new QStackedWidget;
  pagesWidget->addWidget(newPage);
  pagesWidget->addWidget(openPage);

  contentsWidget->setCurrentRow(0);

  //set up the buttons using the built in dialog buttons
  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  //The horizontal layout is the navigation and the stacked widget
  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(contentsWidget);
  horizontalLayout->addWidget(pagesWidget, 1);

  //the main layout is the above layout and the buttons
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(horizontalLayout);
  mainLayout->addWidget(buttonBox);
  setLayout(mainLayout);

  setWindowTitle(tr("Intro Program Options"));
}

/*! This is a helper function for ConfigDialog that creates the items in
    the sidebar widget.
*/
void ConfigDialog::createIcons() {
  QListWidgetItem *newbutton = new QListWidgetItem(contentsWidget);
  newbutton->setIcon(QIcon(":/Images/New.png"));
  newbutton->setText(tr("New"));
  newbutton->setTextAlignment(Qt::AlignHCenter);
  newbutton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem *openbutton = new QListWidgetItem(contentsWidget);
  openbutton->setIcon(QIcon(":/Images/Open.png"));
  openbutton->setText(tr("Open"));
  openbutton->setTextAlignment(Qt::AlignHCenter);
  openbutton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  connect(contentsWidget,
          SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
          this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*)));
}

/*! Helper functino for ConfigDialog that creates the new diagram type
    list widget items.
*/
void ConfigDialog::createNewPage()
{
  // List of widgets
  // There should be one for each diagram type
  doctypeList = new QListWidget(newPage);

  QListWidgetItem *usecaseItem = new QListWidgetItem(doctypeList);
  usecaseItem->setText(tr("Use Case"));

  QListWidgetItem *classItem = new QListWidgetItem(doctypeList);
  classItem->setText(tr("Class"));

  QListWidgetItem *statechartItem = new QListWidgetItem(doctypeList);
  statechartItem->setText(tr("State Chart"));

  QListWidgetItem *collaborationItem = new QListWidgetItem(doctypeList);
  collaborationItem->setText(tr("Collaboration"));
  // end of list of widgets

  //Add the listwidget to a groupbox
  QVBoxLayout *doctypeLayout = new QVBoxLayout;
  doctypeLayout->addWidget(doctypeList);
  QGroupBox *doctypeGroup = new QGroupBox(tr("Choose Diagram Type"));
  doctypeGroup->setLayout(doctypeLayout);

  //add that groupbox to the widget
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(doctypeGroup);
  mainLayout->addSpacing(12);
  newPage->setLayout(mainLayout);
}

/*! Helper function for ConfigDialog that creates the browse button for
    opening up an existing file.
    @todo Change this widget so that the selected filename appears and
          that the browse button only shows xml files.
*/
void ConfigDialog::createOpenPage()
{
  //create the browse button
  QPushButton *openButton = new QPushButton(tr("Open File"), openPage);
  connect(openButton, SIGNAL(clicked()), this, SLOT(browseForFile()));

  QGroupBox *packagesGroup = new QGroupBox(tr("Open File"));

  QGridLayout *packagesLayout = new QGridLayout(openPage);
  packagesGroup->setLayout(packagesLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout(openPage);
  mainLayout->addWidget(packagesGroup);
  mainLayout->addSpacing(12);
  mainLayout->addWidget(openButton);
  mainLayout->addStretch(1);
  openPage->setLayout(mainLayout);
}

/*! This slot updates the currently visible widget in the widget
    stack.
*/
void ConfigDialog::changePage(QListWidgetItem *current,
                              QListWidgetItem *previous) {
  if (!current)
    current = previous;

  pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

/*! This slot opens a file browser to select a file to open.
*/
void ConfigDialog::browseForFile()
{
  fileName = QFileDialog::getOpenFileName(this,
     tr("Open Document"),  tr("XML files (*.xml)"));
}

/*! This overriden slot emits various signals so that the
    MainWindow can pick up on what the user selected in this dialog.
    @todo Have better abstraction on what types of diagrams there are.
          A developer should be be able to add different diagram types at runtime.
*/
void ConfigDialog::accept()
{
  QDialog::accept();

  QListWidgetItem *operation = contentsWidget->currentItem();
  if (operation->text() == "New") {
    QListWidgetItem *selected = doctypeList->currentItem();
    if (selected->text() == "State Chart") {
      emit newDiagramType(BaseNode::StateChart);
    } else if (selected->text() == "Collaboration") {
      emit newDiagramType(BaseNode::Collaboration);
    } else if (selected->text() == "Class") {
      emit newDiagramType(BaseNode::Class);
    } else if (selected->text() == "Use Case") {
      emit newDiagramType(BaseNode::UseCase);
    } else {
      QMessageBox::information(this, "pUML", "Not a valid diagram type!");
    }
  } else if (operation->text() == "Open") {
    QMessageBox::information(this, "pUML", "File opening is not yet implemented (3-6-12)");
    emit openDiagramFile(fileName);
  } else {
    QMessageBox::information(this, "pUML", "Not a valid operation!");
  }

}
