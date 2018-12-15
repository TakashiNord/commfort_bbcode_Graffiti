unit frGraffiti;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, ComCtrls, Buttons;

type
  TGraffitiForm = class(TForm)
    Panel1: TPanel;
    Panel2: TPanel;
    Panel3: TPanel;
    Panel4: TPanel;
    Button1: TButton;
    Button2: TButton;
    Image1: TImage;
    TrackBar1: TTrackBar;
    TrackBar2: TTrackBar;
    Shape1: TShape;
    StaticText1: TStaticText;
    Panel5: TPanel;
    SpeedButton1: TSpeedButton;
    procedure TrackBar1Change(Sender: TObject);
    procedure TrackBar2Change(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    procedure DrawShape(TopLeft, BottomRight:TPoint; AMode:TPenMode);
    procedure initColorPicker() ;
    procedure initColorPickerRun(Sender: TObject) ;
  end;

var
  GraffitiForm: TGraffitiForm;

implementation

{$R *.dfm}


procedure TGraffitiForm.initColorPicker();
var
 allColors: array[0..255] of TColor;
 r,g,b : Integer;
 rt,gt,bt : Integer;
begin
  for r:=0 to 5 do
   for g:=0 to 5 do
 	  for b:=0 to 5 do
    begin
	   rt:=round(r / 5 * 255) ; gt:=round(g / 5 * 255) ; bt := round(b / 5 * 255) ;
	   allColors[r * 36 + g * 6 + b] := TColor ( RGB(rt,gt,bt) ) ;
	  end ;

end;

procedure TGraffitiForm.initColorPickerRun(Sender: TObject);
var
 tmp1: TPanel;
 tmp2: TColor;
begin
  tmp1 := TPanel(Sender);
  tmp2 := TColor (tmp1.Color);
  Image1.Canvas.Pen.Color := tmp2 ;
  Panel5.Color := tmp2 ;
  Shape1.Brush.Color := tmp2 ;
  Panel4.Visible := False ;
  Repaint();
end;

procedure TGraffitiForm.DrawShape(TopLeft:TPoint; BottomRight:TPoint; AMode:TPenMode);
begin
   Image1.Canvas.Pen.Mode := AMode ;
   Image1.Canvas.Pen.Width := TrackBar2.Position;
   Image1.Canvas.LineTo( BottomRight.x , BottomRight.y );
end;

procedure TGraffitiForm.Button1Click(Sender: TObject);
var
 tmpColor :   TColor;
 Rect : TRect ;
begin
 tmpColor := Image1.Canvas.Brush.Color;
 Image1.Canvas.Brush.Color := clWhite ;
 Rect.Left:=0;
 Rect.Top:=0;
 Rect.Right:= Image1.Width;
 Rect.Bottom:= Image1.Height;
 Image1.Canvas.FillRect (Rect);
 // восстанавливаем прежний цвет
 Image1.Canvas.Brush.Color := tmpColor ;
end;

procedure TGraffitiForm.FormCreate(Sender: TObject);
var
  tmp :  TColor ;
begin
 Button1Click(Sender) ;
 TrackBar1.Position:=100 ;
 TrackBar2.Position:=15 ;

 tmp:=TColor(15066597) ;
 Image1.Canvas.Pen.Color := tmp ;
 Panel5.Color := tmp ;
 Shape1.Brush.Color := tmp ;
 Panel4.Visible := False ;
 //initColorPicker();
 Application.ProcessMessages();
 // анимация при появлении
end;

procedure TGraffitiForm.SpeedButton1Click(Sender: TObject);
begin
   if (Panel4.Visible=true) then
     Panel4.Visible:=false
   else
     Panel4.Visible:=true ;
end;

procedure TGraffitiForm.TrackBar1Change(Sender: TObject);
var
  posW : Integer ;
begin
  posW := TrackBar1.Position;
  //posW := Int(255 * posW / 100) ;
  GraffitiForm.AlphaBlend:=true;
  GraffitiForm.AlphaBlendValue:=posW;
  Repaint();
end;

procedure TGraffitiForm.TrackBar2Change(Sender: TObject);
var
  posW : Integer ;
begin
  posW := abs(Shape1.Width-TrackBar2.Position);
  Shape1.Pen.Width := posW div 2 ;
end;

end.
