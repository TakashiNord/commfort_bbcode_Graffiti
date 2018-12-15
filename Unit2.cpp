//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"

// буфер для сохранения изображения при построении фигур
Graphics::TBitmap *ptBmp = NULL;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TForm2::DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode)
{
   int  posW ;
   posW = TrackBar1->Position;
   Image1->Canvas->Pen->Mode  = AMode ;
   Image1->Canvas->Pen->Width = posW;
   Image1->Canvas->LineTo( BottomRight.x , BottomRight.y );
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
  Drawing=true;
  mousepos1=Point(X,Y);
  mousepos2=mousepos1;
  Application->ProcessMessages();
  if (Button == mbLeft) Image1->Canvas->MoveTo(X, Y);
  if (Button == mbRight) Image1->Canvas->LineTo(X, Y);

}
//---------------------------------------------------------------------------

void __fastcall TForm2::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
 if (Drawing) {
	DrawShape(mousepos1, Point(X, Y), pmCopy);
    Drawing = False;
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
 if (Drawing) {
    DrawShape(mousepos1, mousepos2, pmMask);
    mousepos2 = Point(X, Y);
    DrawShape(mousepos1, mousepos2, pmMask);
 }

}
//---------------------------------------------------------------------------


void __fastcall TForm2::Button1Click(TObject *Sender)
{
 TColor tmpColor = Image1->Canvas->Brush->Color;
 Image1->Canvas->Brush->Color = clWhite ;
 //PatBlt(Image1->Canvas->Handle, 0, 0,Image1->Width,Image1->Height, WHITENESS );
 Image1->Canvas->FillRect (TRect(0, 0, Image1->Width, Image1->Height));
 // восстанавливаем прежний цвет
 Image1->Canvas->Brush->Color = tmpColor ;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button2Click(TObject *Sender)
{
 // отправляем на стену
}
//---------------------------------------------------------------------------

void __fastcall TForm2::TrackBar3Change(TObject *Sender)
{
  int posW;
  posW = TrackBar3->Position;
  posW = (int) 255 * posW / 100 ;
  Form2->AlphaBlend=true;
  Form2->AlphaBlendValue=posW;
  Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::TrackBar1Change(TObject *Sender)
{
  int posW;
  posW = TrackBar1->Position;
   //Image1->Canvas->Pen->Width = posW ;
  Shape1->Pen->Width  = abs((Shape1->Width - posW)/2) ;
  //Shape1->Brush->Color =  Panel5->Color ;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm2::FormCreate(TObject *Sender)
{
try  {ptBmp = new Graphics::TBitmap;}
 catch (...)
   {
   //MessageDlg("Can't create a temporary bitmap buffer.",
   //			  mtError, TMsgDlgButtons() << mbOK, 0);
   //exit(1);
   }
 Button1Click(Sender) ;
 TrackBar1->Position=15 ;
 TrackBar3->Position=100 ;

 TColor tmp=(TColor)15066597 ;
 Image1->Canvas->Pen->Color = tmp ;
 Panel5->Color = tmp ;
 Shape1->Brush->Color = tmp ;
 // Говорим Windows, что на нашу форму можно бросать файлы }
 DragAcceptFiles(Handle, True);
 Panel4->Visible = False ;
 initColorPicker();
 Application->ProcessMessages();
 // анимация при появлении
TRect r1,r2;
r1 = Rect(Left+Width/2,Top+Height/2,Left+Width/2,Top+Height/2);
r2 = BoundsRect;
DrawAnimatedRects(Handle,IDANI_CAPTION,&r1,&r2);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormDestroy(TObject *Sender)
{
  if (ptBmp) {delete ptBmp; ptBmp = NULL; }
  DragAcceptFiles(Handle, False);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::SpeedButton1Click(TObject *Sender)
{
   Panel4->Visible=(Panel4->Visible==true)? false : true ;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::WmDropFiles(TWMDropFiles& Message)
{
	HDROP drop_handle=(HDROP)Message.Drop;
	char fName[256];
	int filenum=DragQueryFile(drop_handle,-1,NULL,NULL);
	for (int i=0;i<filenum;i++)
	{
		DragQueryFile(drop_handle,i,fName,256);
		ReadFile(fName);
	}
	DragFinish(drop_handle);
}
//---------------------------------------------------------------------------

void TForm2::ReadFile(AnsiString FileName)
{
	String str=ExtractFileExt(FileName);
	if (str==".bmp")
	{
	 Image1->Picture->LoadFromFile(FileName);
	 //Image1->Width = Image1->Picture->Width;
	 //Image1->Height = Image1->Picture->Height;
	}
	if (str==".ico")
	{
	  TIcon      *tIcon;
	  tIcon = new TIcon;
	  tIcon->LoadFromFile(FileName);
	  if (ptBmp) {
		//ptBmp->Assign(Image1->Picture->Graphic);
		//ptBmp->Height = tIcon->Height;
		//ptBmp->Width = tIcon->Width;
		//Image1->Picture->Assign(ptBmp);
	  }
	  delete tIcon;
	  Image1->Picture->LoadFromFile(FileName);
	}
	if (str==".jpg" || str==".jpeg")
	{
	  Image1->AutoSize=true;
	  Image1->Picture->LoadFromFile(FileName);
	  if (ptBmp) { ptBmp->Assign(Image1->Picture->Graphic); }
	}
	if (str==".tiff")
	{

	}
	if (str==".png")
	{

	}
	if (str==".gif")
	{

	}

}
//---------------------------------------------------------------------------

void TForm2::initColorPicker()
{
  TColor allColors[255];
  int r,g,b ;
  int rt,gt,bt ;
  for(r=0;r<6;r++)
  {
   for(g=0;g<6;g++)
   {
	 for(b=0;b<6;b++)
	 {
	   rt=int(r / 5. * 255.) ; gt=int(g / 5. * 255.) ; bt = int(b / 5. * 255.) ;
	   allColors[r * 36 + g * 6 + b] = TColor ( RGB(rt,gt,bt) ) ;
	 }
	}
  }

  int n ;
  int i,j ;
  int startX = 0, startY = 0 ;
  TPanel *P[255];
  for(j=0;j<12;j++)
  {
   for(i=0;i<18;i++)
   {
	 r = floor(i / 6) + 3 * floor(j / 6);
	 g = i % 6;
	 b = j % 6;
	 n = r * 36 + g * 6 + b;

	 P[n]=new TPanel(this->Panel4);
	 P[n]->OnClick = initColorPickerRun;
	 P[n]->Left = i * 14 + startX;
	 P[n]->Top = j * 14 + startY;
	 P[n]->Height = 14;
	 P[n]->Width = 14;
	 P[n]->Visible= true;
	 P[n]->Parent =  this->Panel4;
	 P[n]->Name = "P" + AnsiString(allColors[n]) ;
	 P[n]->Color = (TColor)allColors[n];
	 P[n]->DoubleBuffered=true;
	 P[n]->Enabled=true;
	 P[n]->FullRepaint=true;
	 P[n]->ParentColor=false;
	 P[n]->ShowCaption=false;
	 P[n]->ParentBackground=false; // !!!!!!!!!!!!!!!!!!!!!

   }
  }
}
//---------------------------------------------------------------------------
void  __fastcall TForm2::initColorPickerRun(TObject *Sender)
{
  TPanel *tmp = (TPanel *) Sender;
  Image1->Canvas->Pen->Color = (TColor) tmp->Color ;
  Panel5->Color = (TColor) tmp->Color ;
  Shape1->Brush->Color =  (TColor) tmp->Color ;
  Panel4->Visible = False ;
  Repaint();
}
//---------------------------------------------------------------------------


