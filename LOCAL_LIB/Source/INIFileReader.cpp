
#include "stdafx.h"
#include <INIFileReader.h>


CINIFileReader::CINIFileReader(
    LPCTSTR pszIniFile
    )
{
    if ( pszIniFile != NULL )
    {
        _tcscpy_s( m_szIniFile, _countof (m_szIniFile), pszIniFile );
    }
    else
    {
        m_szIniFile[ 0 ] = '\0';
    }
}


void
CINIFileReader::SetIniFile(
    LPCTSTR pszIniFile
    )
{
    _tcscpy_s( m_szIniFile, _countof (m_szIniFile), pszIniFile );
}


int CINIFileReader::ReadInt( LPCTSTR section, LPCTSTR key, int DefaultValue )
{
    int retval = 0;
    
    retval = GetPrivateProfileInt(
        section,
        key,
        DefaultValue,
        m_szIniFile);

    return retval;
}

float CINIFileReader::ReadFloat( LPCTSTR section, LPCTSTR key, float DefaultValue )
{
    TCHAR szBuf[ 50 ] = _T( "" );

    GetPrivateProfileString(
        section,
        key,
        _T( "" ),
        szBuf,
        50,
        m_szIniFile);

    float retVal = DefaultValue;
    if ( szBuf[ 0 ] != '\0' )
    {
        _stscanf_s( szBuf, _T( "%f" ), &retVal );
    }
    return retVal;
}

float CINIFileReader::ReadBrace4Float( LPCTSTR section, LPCTSTR key, float *fArray, float DefaultValue )
{
    TCHAR szBuf[ 50 ] = _T( "" );

    GetPrivateProfileString(
        section,
        key,
        _T( "" ),
        szBuf,
        50,
        m_szIniFile);

    float retVal = DefaultValue;
    if ( szBuf[ 0 ] != '\0' )
    {
        _stscanf_s( szBuf, _T( "%f" ) _T( "%f" ) _T( "%f" ) _T( "%f" ), &fArray[0], &fArray[1], &fArray[2], &fArray[3] );
    }
    return retVal;
}

int CINIFileReader::ReadString( LPCTSTR section, LPCTSTR key, LPTSTR pBuf, int iSizeBuf, LPCTSTR DefaultValue )
{
    int retval = 0;
    retval = GetPrivateProfileString(
        section,
        key,
        DefaultValue,
        pBuf,
        iSizeBuf,
        m_szIniFile);

    //fail conditions
    if ((retval == (iSizeBuf - 1)) || (retval == (iSizeBuf - 2)))
    {
        retval = READ_STRING_FAIL;
    }
    else    
    {
        retval = 0;
    }

    return retval;
}

//char *Primitive::clear_white_space(char *chptr,ifstream &fin,char *buff)
//{
//	while((*chptr=='\0')&&(!fin.eof())){
//		fin.getline(buff,80);
//		chptr=buff;
//		if (*chptr !='\0'){
//			while(((*chptr==' ')||(*chptr=='\t'))&&(*chptr!='\0')){
//				chptr++;
//			}
//			if (*chptr!='-') chptr=buff;
//		}
//	}
//	if(fin.eof()) return 0;
//	while((*chptr==' ')||(*chptr=='\t')) {chptr++;}
//	return chptr;
//}
////*************************************************************************
////
////************************************************************************
//
//Primitive::WordType Primitive::check_word(char *word){
//
//	if (!strcmp(word,"Coordinate3")){
//		return COORDINATE_3;
//	}
//	else if (!strcmp(word,"IndexedTriangleStripSet")){
//		return IDX_TRI_STIP_SET;
//	}
//	else if (!strcmp(word,"IndexedFaceSet")){
//		return IDX_FACE_SET;
//	}
//	else if (!strcmp(word,"point")){
//		return POINT;
//	}
//	else if (!strcmp(word,"coordIndex")){
//		return COORD_IDX;
//	}
//	else if (!strcmp(word,"materialIndex")){
//		return MATERIAL_IDX;
//	}
//	else if (!strcmp(word,"normalIndex")){
//		return NORMAL_IDX;
//	}
//	else if (!strcmp(word,"TextureCoordIndex")){
//		return TEX_COORD_IDX;
//	}
//	else if (!strcmp(word,"#")){
//		return COMMENT;
//	}
//	else if (!strcmp(word,"{")){
//		return LEFT_BRACE;
//	}
//	else if (!strcmp(word,"[")){
//		return LEFT_BRACKET;
//	}
//	else if (!strcmp(word,"]")){
//		return RIGHT_BRACKET;
//	}
//	else if (!strcmp(word,"}")){
//		return RIGHT_BRACE;
//	}
//	else if (!strcmp(word,",")){
//		return COMMA;
//	}
//	else if (!strcmp(word,"DONE")){
//		return DONE;
//	}
//	else if (!strcmp(word,"ColorSet")){
//		return COLOR_SET;
//	}
//	else if (!strcmp(word,"color")){
//		return COLOR;
//	}
//	else if (!strcmp(word,"TransformSet")){
//		return TRANSFORM_SET;
//	}
//	else if (!strcmp(word,"viewer")){
//		return VIEWER;
//	}
//	else if (!strcmp(word,"perspective")){
//		return PERSPECTIVE;
//	}
//	else if (!strcmp(word,"screen")){
//		return SCREEN;
//	}
//		return NONE;
//
//}
////*************************************************************************
////
////************************************************************************
//char *Primitive::get_word(char *chptr,ifstream &fin,char *buff,char *new_word){
//	char *new_char=new_word;
//	int index=0;
//	new_word[index]='\0';
//
//	if (chptr=clear_white_space(chptr,fin,buff)){
//		if ((!isdigit(*chptr))&&(*chptr!='-')){
//			while ((*chptr!=' ')&&(*chptr!='\t')&&(*chptr!='\0')&&
//				   (!check_word(new_word))){
//				new_word[index]=*chptr;
//				new_word[index+1]='\0';
//				chptr++;
//				index++;
//			}
//		}else{
//			while ((isdigit(*chptr))||(*chptr=='.')||(*chptr=='-')){
//				new_word[index]=*chptr;
//				new_word[index+1]='\0';
//				chptr++;
//				index++;
//			}
//		}
//
//	}
//	if(new_word[0]=='#'){
//		fin.getline(buff,80);
//		chptr = buff;
//	}
//	return chptr;
//}
////*************************************************************************
////
////************************************************************************
//
//int Primitive::make_decision(char*word,WordTracker &WT,PrimInf &TX){
//	WordType  CurrentWord;
//	CurrentWord = check_word(word);
//
//	switch(CurrentWord){
//		 
//		case COORDINATE_3:
//			WT.Coodinate3++;
//				break;
//		case IDX_TRI_STIP_SET:
//			WT.IdxTriStipSet++;
//				break;
//		case IDX_FACE_SET:
//			WT.IdxFace_Set++;
//				break;
//		case COLOR_SET:
//			WT.Color_Set++;
//				break;
//		case TRANSFORM_SET :
//			WT.Transform_Set++;
//				break;
//		case POINT:
//			WT.Point++;
//				break;
//		case COLOR:
//			WT.Color++;
//				break;
//		case SCREEN: 
//			WT.ScreenTransform++;
//				break;
//		case VIEWER: 
//			WT.ViewTransform++;
//				break;
//		case PERSPECTIVE:
//			WT.PerspectiveTransform++;
//				break;
//		case COORD_IDX:	
//			WT.CoordIdx++;
//				break;
//		case MATERIAL_IDX:
//			WT.MaterialIdx++;
//				break;
//		case NORMAL_IDX:	
//			WT.Normal_Idx++;
//				break;
//		case TEX_COORD_IDX: 
//			WT.Tex_Coord_Idx++;
//				break;
//		case LEFT_BRACE:
//			WT.LeftBrace++;
//				break;
//		case LEFT_BRACKET:
//			WT.LeftBracket++;
//				break;
//		case RIGHT_BRACKET:	
//			if(WT.MaterialIdx){
//				WT.MaterialIdx--;
//			}else if(WT.Normal_Idx){
//				WT.Normal_Idx--;
//			}else if(WT.Tex_Coord_Idx){
//				WT.Tex_Coord_Idx--;
//			}else if(WT.Point){
//				WT.Point--;
//			}else if(WT.Color){
//				WT.Color--;
//			}else if(WT.ScreenTransform){
//				WT.ScreenTransform--;
//			}else if(WT.ViewTransform){
//				WT.ViewTransform--;
//			}else if(WT.PerspectiveTransform){
//				WT.PerspectiveTransform--;
//			}else if(WT.CoordIdx){	
//				WT.CoordIdx--;
//			}else{
//				cout <<"Error" <<endl;
//			}
//				break;
//		case RIGHT_BRACE:	
//			if(WT.Coodinate3){
//			}else if(WT.IdxTriStipSet){	
//				WT.IdxTriStipSet--;			
//			}else if(WT.IdxFace_Set){
//				WT.IdxFace_Set--;				
//			}else if(WT.Color_Set){
//				WT.Color_Set--;				
//			}else if(WT.Transform_Set){
//				WT.Transform_Set--;				
//			}else{
//				cout <<"Error" <<endl;
//			}			
//		break;
//		case COMMENT:	
//			WT.Comment++;
//				break;
//		case COMMA: 
//				break;
//		case DONE:
//				break;
//		default :
//			if(WT.Coodinate3||WT.IdxFace_Set||WT.IdxTriStipSet||WT.Color_Set
//			   ||WT.Transform_Set)
//			{
//				if(WT.Point)
//				{
//					switch(WT.CoordCnt%3){
//					case 0:  
//						WT.PointCnt++;
//					    WT.point=new Point;
//					    WT.point->set_x(atof(word));
//					  	break;
//					case 1:
//					    WT.point->set_y(atof(word));
//						break;
//					case 2:
//					    vertcount=WT.point->get_vertex_number();
//					    WT.point->set_z(atof(word));
//					    vertere[vertcount]=WT.point;
//					    WT.point=0;
//						break;
//					}
//					WT.CoordCnt++;
//				}else if(WT.CoordIdx)
//				{
//				    coordIndex[WT.TriIdxCount]=atoi(word);
//					WT.TriIdxCount++;
//				}else if(WT.MaterialIdx)
//				{
//				}else if(WT.Normal_Idx)
//				{
//				}else if(WT.Tex_Coord_Idx)
//				{
//				}else if(WT.ScreenTransform)
//				{
//					switch(WT.TransParamCnt % 5){
//						case 0:
//							WT.TransParamCnt++;
//							TX.xmin = atof(word);
//							break;
//						case 1:
//							WT.TransParamCnt++;
//							TX.ymin = atof(word);
//							break;
//						case 2:
//							WT.TransParamCnt++;
//							if(TX.xmax < 0)
//								TX.xmax = atof(word);
//							break;
//						case 3:
//							WT.TransParamCnt++;
//							if(TX.ymax < 0)
//								TX.ymax = atof(word);
//							break;
//						case 4:
//							WT.TransParamCnt=0;
//							TX.zmax = atof(word);
//							break;
//					}
//				}else if(WT.ViewTransform){
//
//					switch(WT.TransParamCnt % 9){
//						case 0:
//							WT.TransParamCnt++;
//							TX.xc = atof(word);
//							break;
//						case 1:
//							WT.TransParamCnt++;
//							TX.yc = atof(word);
//							break;
//						case 2:
//							WT.TransParamCnt++;
//							TX.zc = atof(word);
//							break;
//						case 3:
//							WT.TransParamCnt++;
//							TX.xf = atof(word);
//							break;
//						case 4:
//							WT.TransParamCnt++;
//							TX.yf = atof(word);
//							break;
//						case 5:
//							WT.TransParamCnt++;
//							TX.zf = atof(word);
//							break;
//						case 6:
//							WT.TransParamCnt++;
//							TX.xup = atof(word);
//							break;
//						case 7:
//							WT.TransParamCnt++;
//							TX.yup = atof(word);
//							break;
//						case 8:
//							WT.TransParamCnt=0;
//							TX.zup = atof(word);
//							break;
//					}
//				
//				}else if(WT.PerspectiveTransform){
//					switch(WT.TransParamCnt % 4){
//						case 0:
//							WT.TransParamCnt++;
//							TX.aspect = atof(word);
//							break;
//						case 1:
//							WT.TransParamCnt++;
//							TX.fov = atof(word);
//							break;
//						case 2:
//							WT.TransParamCnt++;
//							TX.pnear = atof(word);
//							break;
//						case 3:
//							WT.TransParamCnt=0;
//							TX.pfar = atof(word);
//							break;
//					}
//				
//				}else if(WT.Color)
//				{
//					switch(WT.ColorCnt % 3)
//					{
//					case 0:
//						WT.colors[0] = atoi(word);
//						break;
//					case 1:
//						WT.colors[1] = atoi(word);
//						break;
//					case 2:
//						WT.colors[2] = atoi(word);
//						vertere[(int)(WT.ColorCnt / 3)]->set_color(WT.colors);
//						break;
//					}
//					WT.ColorCnt++;
//				}
//			}
//			
//				break;
//	}
//	return 1;
//}
//
//
////*************************************************************************
////    Initiate File
////************************************************************************
//
//
//
//int Primitive::init_file(char *file,PrimInf &tx){
//
// char buf[80],*chptr,new_word[80],*wordptr;
// int wordindex=0;
// chptr=buf;
// buf[0]='\0';
// wordptr=new_word;
// new_word[0]='\0';
//
//
// ifstream fin;
// fin.open(file);
// if (!fin){
//  cout<<endl<<"file was not opened"<<endl;
//  exit(0);
//  }
//
//
// WordTracker WT;
//
// while(!fin.eof()){
//  if(chptr=get_word(chptr,fin,buf,new_word)){
//   make_decision(new_word,WT,tx);
//  }
// }
//
//
// fin.close();
// return 1;
//}
