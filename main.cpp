#include "TF1.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include <iostream>
#include "new.h"

int main(int argc, char **argv)
{
   hello();
   TApplication app("app", &argc, argv);
   TCanvas* c = new TCanvas("c", "Something", 0, 0, 800, 600);
   TF1 *f1 = new TF1("f1","sin(x)", -5, 5);
   f1->SetLineColor(kBlue+1);
   f1->SetTitle("My graph;x; sin(x)");
   f1->Draw();
   c->Modified(); c->Update();
   TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
   rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
   app.Run();
   return 0;
}