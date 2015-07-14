/**
				Video Resumen Open Source
						2015
Authors:
		Ponzoni Nelson, npcuadra <at> gmail.com
		Olivera Jose, joseolivera123 <at> gmail.com
		Oliva Eduardo, edumoliva <at> gmail.com

Facultad de Ingenieria y Ciencias Hidricas
Universidad Nacional del Litoral

Procesamiento Digital de Imagenes, 2015


please replace <at> for arroba
**/

#include "opencv2/opencv.hpp"
#include <cmath>
#include <algorithm>
#include <vector>

using namespace cv;
using namespace std;

struct BlobCenter{
	vector<Point2i> blob;
	Point2i center;	
	int id;
	int frame; // en que frame aparece
};


/*
- maskMog2 -> mascarita de mog2
- src -> imagen achicada
- frame2 -> sin blur
- output -> devuelve la salida a color del frame con el convexhull pasado
- threshold_output -> devuelve la salida biaria del frame con el convexhull pasado
*/
void ConvexHull(const Mat maskMog2,Mat src,const Mat frame2, Mat &output, Mat &threshold_output)
{
	RNG rng(12345);
	int thresh = 10;
	Mat src_gray;
	cvtColor( maskMog2, src_gray, CV_BGR2GRAY );
	
	Mat src_copy = maskMog2.clone();
	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	/// Detect edges using Threshold
	threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
	
	/// Find contours
	findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
	/// Find the convex hull object for each contour
	vector<vector<Point> >hull( contours.size() );
	
	for( int i = 0; i < contours.size(); i++ )
		convexHull( Mat(contours[i]), hull[i], false );
	
	/// Draw contours + hull results
	Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	
	for( int i = 0; i< contours.size(); i++ ){
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( src, hull, i, color, -1, 8, vector<Vec4i>(), 0, Point() );
	}
	
	cvtColor( src, src_gray, CV_BGR2GRAY );
	threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
	
	resize(frame2, src, Size(frame2.size().width/2, frame2.size().height/2) );
	output=src;
	
	for(int i = 0; i < 3; i++)
		for (int x=0; x<maskMog2.cols; x++)
			for (int y=0; y<maskMog2.rows; y++)
				output.at<cv::Vec3b>(y,x)[i] = 1 - src.at<cv::Vec3b>(y,x)[i] * threshold_output.at<uchar>(y,x);
}


/*
- binary -> imagen binaria
- blobs -> vector que contiene las coordenadas de cada objeto de la imagen
*/
void FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs)
{
	blobs.clear();
	
	// Fill the label_image with the blobs
	// 0  - background
	// 1  - unlabelled foreground
	// 2+ - labelled foreground
	
	cv::Mat label_image;
	binary.convertTo(label_image, CV_32SC1);
	
	int label_count = 2, // starts at 2 because 0,1 are used already
		minArea = TAM_MIN_OBJETOS;  //min area of the blob
	
	for(int y=0; y < label_image.rows; y++) {
		int *row = (int*)label_image.ptr(y);
		for(int x=0; x < label_image.cols; x++) {
			if(row[x] != 1) {
				continue;
			}
			
			cv::Rect rect;
			cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 4);
			
			std::vector <cv::Point2i> blob;
			
			for(int i=rect.y; i < (rect.y+rect.height); i++) {
				int *row2 = (int*)label_image.ptr(i);
				for(int j=rect.x; j < (rect.x+rect.width); j++) {
					if(row2[j] != label_count) {
						continue;
					}
					
					blob.push_back(cv::Point2i(j,i));
				}
			}
			
			if(blob.size() > minArea)
				blobs.push_back(blob);
			
			label_count++;
		}
	}
}


/*
- img -> imagen original
- blobs_out -> vectot < BlobCenter > de img
*/
void  Img2Blob(const cv::Mat &img, vector< BlobCenter > &blobs_out, int frame )
{
	Mat binary;
	
	vector < vector<Point2i > > blobs;
	threshold(img, binary, 0.0, 1.0, cv::THRESH_BINARY);
	
	FindBlobs(binary, blobs);
	
	double aux_x;
	double aux_y;
	int Size;
	int id_blob = 1;
	
	// calculo del centro de masa
	for(size_t i=0; i < blobs.size(); i++) {	// cada etiqueta
		aux_x = 0;
		aux_y = 0;
		BlobCenter  Bc;
		Bc.blob = blobs[i];
		Size = Bc.blob.size(); //cantidad de puntos dentro del blob
		if(Size < TAM_MAX_OBJETOS){//si el objeto muy grande lo deja de lado 
			for(size_t j=0; j < Size; j++) {
				Bc.center.x += (double)Bc.blob[j].x;
				Bc.center.y += (double)Bc.blob[j].y;
			}
			
			Bc.center.x /= Size;
			Bc.center.y /= Size;
			
			Bc.id = id_blob;
			Bc.frame = frame;
			id_blob++;
			
			blobs_out.push_back(Bc);			
		}
		
	}
	
}

/*
POR EL MOMENTO NO SE USA POR ESO LA DEJO COMENTADA
*/
//void CenterDetector(Mat &threshold_output, Mat &out,vector<KeyPoint> &keyPoints)
//{
//	SimpleBlobDetector::Params params;
//	
//	params.minThreshold = 40;
//	params.maxThreshold = 60;
//	params.thresholdStep = 5;
//	
//	params.minArea = 100; 
//	params.minConvexity = 0.3;fundar
//	params.minInertiaRatio = 0.01;
//	
//	params.maxArea = 8000;
//	params.maxConvexity = 10;
//	
//	params.filterByColor = false;
//	params.filterByCircularity = false;
//	
//	line( threshold_output, Point(0, threshold_output.rows-1), Point( threshold_output.cols-1, threshold_output.rows-1 ), Scalar::all(255) );
//	
//	SimpleBlobDetector blobDetector( params );
//	blobDetector.create("SimpleBlob");
//	
//	blobDetector.detect( threshold_output, keyPoints );
//	drawKeypoints( threshold_output, keyPoints, out, CV_RGB(0,255,0), DrawMatchesFlags::DEFAULT);
//	
//}


/*
verifica que un id-viejo tenga asociado un único id-nuevo (el más cercano)
*/
void verificar_etiqueta(int id1,int id2, double min_dist_centers, vector< vector<double> > &etiquetas)
{
	bool flag = true;
	
	if(id1 != -1){
		for(int i=0; i<etiquetas.size(); i++){
			if(etiquetas[i][0] == (double)id1){
				if(min_dist_centers < etiquetas[i][2]){
					etiquetas[i][2] = min_dist_centers;
					etiquetas[i][1] = id2; 
				}
				flag = false;
				break;
			}
		}
		
		if(flag){
			vector<double> aux;
			aux.push_back(id1);
			aux.push_back(id2);
			aux.push_back(min_dist_centers);
			etiquetas.push_back(aux);
		}
	}
	
}


/*
b1 -> blobs_frame_old
b2 -> blobs_frame
etiquetas -> correspondencia entre id nuevo y viejo: [id_blob_old, id_blob_new, distancia_entre_centros]
*/
void correspondencias_id(vector < BlobCenter > &b1, vector < BlobCenter > &b2, vector< vector<double> > &etiquetas)
{	
	bool flag;
	double dist_centers, min_dist_centers;
	int min_dist_id;
	
	for (size_t i=0; i<b2.size(); i++){
		min_dist_centers = 0;
		min_dist_id = -1;
		flag = true;
		
		for (size_t j=0; j<b1.size(); j++){
			dist_centers = sqrt( pow(((double)b2[i].center.x - (double)b1[j].center.x), 2) +
				pow(((double)b2[i].center.y - (double)b1[j].center.y), 2) );
			
			if(flag){
				flag = false;
				min_dist_centers = dist_centers;
				min_dist_id = b1[j].id;
			}
			else if(dist_centers < min_dist_centers){
				min_dist_centers = dist_centers;
				min_dist_id = b1[j].id;
			}
		}
		
		//verifico que no haya otro id2 asociado al id1 mas cercano identificado
		verificar_etiqueta(min_dist_id, b2[i].id, min_dist_centers, etiquetas);
	}	
}


/*
actualiza las etiquetas de los blobs del frame actual con las correspondidas del frame anterior
y asigna etiquetas nuevas y únicas a los nuevos blobs dentro del frame
*/
void reEtiquetado(vector< BlobCenter > &blobs, vector< vector<double> > &etiquetas, int &next_id)
{
	bool found; //si esta bandera es falsa, el blob es un nuevo objeto en el frame
	
	for(int i=0; i<blobs.size(); i++){
		
		found = false;
		
		for(int j=0; j<etiquetas.size(); j++){
			if(blobs[i].id == etiquetas[j][1]){
				found = true;
				blobs[i].id = etiquetas[j][0];
				break;
			}
		}
		
		if(!found){
			blobs[i].id = next_id;
			next_id = next_id + 1;
		}
	}
	
}

/**
Colorea los blobs y los devuelve en una imagen
@param blobs vector<BlobCenter> vector con todos los centros, ids, label, ...
@param output Mat imagen de salida con los blobs pintados
@param time double tiempo donde ocurre el suceso
**/
void paintBlobs(const vector < BlobCenter > &blobs, Mat &output, double time)
{
	std::vector<int> labelCheck;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	
	for(size_t i=0; i < blobs.size(); i++) {
		
		vector<Point2i> blob = blobs[i].blob;
		int label = blobs[i].id;
		
		r = label*100/1.0;
		g = label*100/10.0;
		b = label*100/100.0;
		
		for(int j=0; j<blob.size(); j++){
			output.at<cv::Vec3b>(blob[j].y,blob[j].x)[0] = b;
			output.at<cv::Vec3b>(blob[j].y,blob[j].x)[1] = g;
			output.at<cv::Vec3b>(blob[j].y,blob[j].x)[2] = r;
		}
		
#if MAS_INFO
//		string etiqueta = to_string(label);
		string etiqueta = to_string(time);
		double escala = 0.3f;
		double grosor = 1;
//		FONT_HERSHEY_SCRIPT_SIMPLEX
//		FONT_HERSHEY_PLAIN
		putText(output,etiqueta, cvPoint(blobs[i].center.x,blobs[i].center.y), FONT_HERSHEY_SCRIPT_SIMPLEX,	escala,	cvScalar(255,255,255), grosor);
#endif	
	}
	
}
//
//

void FillObjects(const Mat &src, vector< BlobCenter > &blobs, vector <pair <vector<vector <int> > , vector<Mat> > > &Objects)
{
	
	for(int i=0; i<blobs.size(); i++){
		
		Mat obj_img = Mat::zeros(src.size(), CV_8UC3);
		vector<Point2i> blob = blobs[i].blob;
		int id = blobs[i].id;
		
		for(int j=0; j<blob.size(); j++){
			//pinto en obj_img lo que haya en src en las coordenadas del blob
			obj_img.at<Vec3b>(blob[j].y, blob[j].x)[0] = src.at<Vec3b>(blob[j].y, blob[j].x)[0]; //b
			obj_img.at<Vec3b>(blob[j].y, blob[j].x)[1] = src.at<Vec3b>(blob[j].y, blob[j].x)[1]; //g
			obj_img.at<Vec3b>(blob[j].y, blob[j].x)[2] = src.at<Vec3b>(blob[j].y, blob[j].x)[2]; //r
		}
		
		if(id < Objects.size()){
			vector <int> aux;
			aux.push_back(blobs[i].center.x);
			aux.push_back(blobs[i].center.y);
			aux.push_back(blobs[i].frame);
			Objects[id].first.push_back(aux);
			Objects[id].second.push_back(obj_img);
		}
		else{ //el primer frame en el vector siempre es negro completo
			vector<Mat> aux;
			
			vector <int> aux2;
			aux2.push_back(0);
			aux2.push_back(0);
			aux2.push_back(0);

			
			while(id >= Objects.size()){
				pair <vector<vector <int> > , vector<Mat> > otro;
				otro.first.push_back(aux2);
				otro.second = aux;
				Objects.push_back(otro);
			}
			
			vector <int> aux3;
			aux3.push_back(blobs[i].center.x);
			aux3.push_back(blobs[i].center.y);
			aux3.push_back(blobs[i].frame);
			Objects[id].first.push_back(aux3);
			Objects[id].second.push_back(obj_img);
		}
	}
	
}
void FillObjects2(const Mat &src, vector< BlobCenter > &blobs, vector< vector< Mat > > &Objects)
{
	
	for(int i=0; i<blobs.size(); i++){
		
		Mat obj_img = Mat::zeros(src.size(), CV_8UC3);
		vector<Point2i> blob = blobs[i].blob;
		int id = blobs[i].id;
		
		for(int j=0; j<blob.size(); j++){
			//pinto en obj_img lo que haya en src en las coordenadas del blob
			obj_img.at<Vec3b>(blob[j].y, blob[j].x)[0] = src.at<Vec3b>(blob[j].y, blob[j].x)[0]; //b
			obj_img.at<Vec3b>(blob[j].y, blob[j].x)[1] = src.at<Vec3b>(blob[j].y, blob[j].x)[1]; //g
			obj_img.at<Vec3b>(blob[j].y, blob[j].x)[2] = src.at<Vec3b>(blob[j].y, blob[j].x)[2]; //r
		}
		
		if(id < Objects.size()){
			Objects[id].push_back(obj_img);
		}
		else{ //el primer frame en el vector siempre es negro completo
			vector<Mat> aux;
			
			while(id >= Objects.size()){
				Objects.push_back(aux);
			}
			
			Objects[id].push_back(obj_img);
		}
	}
	
}
//

/**
recibe una mascara, de tres canales y donde tiene [0,0,0] reemplaza en 
esa posicion por el valor de la imagen 
**/
void reemplaza(Mat &fondo, Mat &imagen){
	for (int i=0;i<fondo.cols;i++){
		for (int j=0;j<fondo.rows;j++){
			if(!((imagen.at<Vec3b>(j, i)[0]==0) && (imagen.at<Vec3b>(j, i)[1]==0) && (imagen.at<Vec3b>(j, i)[2]==0))){
				fondo.at<Vec3b>(j, i)[0] = imagen.at<Vec3b>(j, i)[0]; //b
				fondo.at<Vec3b>(j, i)[1] = imagen.at<Vec3b>(j, i)[1]; //g
				fondo.at<Vec3b>(j, i)[2] = imagen.at<Vec3b>(j, i)[2]; //r
			}
		}
	}
	
}
//
/**
mostrar y guardar en video la salida de los eventos



forma de almacenamiento:

auto1 : frame1 fram2 frame3....
auto2 : frame1 fram2 ....
auto3 : frame1 fram2 frame4 frame5....
auto4
....
**/
///ACORDARSE DE MANDAR EL FONDO CHIQUITO
void mostrar(const vector <pair <vector<vector <int> > , vector<Mat> > > &Objects2, const Mat &fondo, int ventana, VideoWriter &outputVideo, int FPS)
{
	vector <pair <vector<vector <int> > , vector<Mat> > > Objects = Objects2;
	vector <pair <vector<vector <int> > , vector<Mat> > > window;
	
	int count = DELAY_EVENTOS; // espera entre frames para agregar objetos a la escena
	
	// para poder trabajar con estilo de cola
	reverse(Objects.begin(),Objects.end());

	while(true)
	{
		if (!Objects.empty()) // si hay cosas que mostrar
		{
			Mat frame = fondo.clone();
			
			// si la ventana (de objetos) le queda lugar y si hay objetos. Tengo permiso de frames (delay) para agregar
			while((window.size()<ventana || window.size()<Objects.size()) && (count >= DELAY_EVENTOS))
			{
				window.insert(window.begin(),*(--Objects.end()));
				Objects.pop_back();
				count = 0; // inserte un nuevo objeto, contador de frames a cero!
			}
			
			for(int i = 0 ; i < window.size(); i++)
			{
				reemplaza(frame,*(window[i].second.begin()));
				
				vector< vector <int> > aux2 = window[i].first;
				vector<int> aux = aux2[0];
				string etiqueta = to_string(aux[2]/FPS); // en el tercer lugar guarda los frames, y en la primera y segunda la posicio del centro
				double escala = 0.3f;
				double grosor = 1.5f;
#if EVENTOS_SEGUNDOS
				putText(frame,etiqueta, cvPoint(aux[0],aux[1]), FONT_HERSHEY_SCRIPT_SIMPLEX,	escala,	cvScalar(0,0,255), grosor);
#endif
				window[i].first.erase(window[i].first.begin());
				window[i].second.erase(window[i].second.begin());
				
				if(window[i].second.empty()){
					window.erase(window.begin()+i);
				}
			}

#if MOSTRAR_RESULTADO
					
			
			imshow("Resultado (ESC para cortar)",frame);
#endif
			
			count++; // ya tengo el frame, aumento el contador para dar espacio a uno mas
			
			// almacenamiento en video
			if (!outputVideo.isOpened())
			{
				cout  << "\n\n\t NO SE PUEDE ESCRIBIR EN EL ARCHIVO!! " << endl;
				return;
			}
			else{
				outputVideo.write(frame); // guarda en disco
			}
			
			if (waitKey(50) >= 0)   // escape, corta el video
				break;
			
		}
		else {
			cout << "\n\n\t No hay mas eventos! Saliendo... " << endl;
			outputVideo.release();
			return;
		}
	}
}
//
//
/**
calcula el fondo, en frame actual es el que lee, en mascara estan los 
autos en 1 y el fondo en 0, fondo temporal va el fondo que llevo calculado, salida es la "salida"
**/
void extraerFondo(const Mat &frame_actual, const Mat &mascara, const Mat &fondo_temporal,Mat &salida)
{
	salida = fondo_temporal;	// el fondo temporal va a ser la salida actualizada
	double alfa = FONDO_MERGE;	// parametro global para el mezclado del fondo
	double beta = 1.0 - alfa;
	
	for (int i=0; i<mascara.cols; i++){
		for (int j=0;j<mascara.rows;j++){
			if( (!(mascara.at<uchar>(j, i)) )){
				// cualquier objeto es 1, fondo es 0. entra a la ponderacion cualquier cosa distinta a un objeto
				salida.at<Vec3b>(j, i)[0] = (alfa * salida.at<Vec3b>(j, i)[0]) + (beta * frame_actual.at<Vec3b>(j, i)[0]); //b
				salida.at<Vec3b>(j, i)[1] = (alfa * salida.at<Vec3b>(j, i)[1]) + (beta * frame_actual.at<Vec3b>(j, i)[1]); //g
				salida.at<Vec3b>(j, i)[2] = (alfa * salida.at<Vec3b>(j, i)[2]) + (beta * frame_actual.at<Vec3b>(j, i)[2]); //r
			}
		}
	}
}
//
//
/**
selecciona los objetos por determinado criterio
**/
void seleccionar(const vector <pair <vector<vector <int> > , vector<Mat> > > &Objects,vector <pair <vector<vector <int> > , vector<Mat> > > &Objects_aux)
{
	Objects_aux.clear();
	/// por ahora tomo el color azul
	
	for (int i=0; i<Objects.size(); i++){
		int contador = 0;
		bool bandera = false;
		
		vector<Mat> objeto = Objects[i].second;
		for (int j=0; j<objeto.size(); j++){
			
			Mat ig;
			
//			http://blog.damiles.com/2010/01/segmentation-object-detection-by-color/
			inRange(objeto[j],Scalar(0,0,0),Scalar(10,10,10),ig); 
			double tam_obj = objeto[j].cols*objeto[j].rows - countNonZero(ig); // obtengo la cantidad de pixeles que componen al objeto
			
			if (tam_obj>TAM_MIN_OBJETOS*10)	// si es mayor a 10 veces el minimo
			{
				Mat obj_hsv,img1;
				cvtColor(objeto[j],obj_hsv,CV_RGB2HSV); // paso un frame a hsv
				Mat imgSplit[3];
#if SEG_ROJO_AZUL
				unsigned char rangoIni = 90;
				unsigned char rangoFin = 130;
#else
				unsigned char rangoIni = 50;
				unsigned char rangoFin = 90;
#endif
				
				inRange(obj_hsv,Scalar(rangoIni,130,130),Scalar(rangoFin,255,255),img1); // mascara rojo con negro

				double tam_color = countNonZero(img1); // cuento la cantidad de azules

//				imshow("asd",objeto[j]);imshow("asd2",img1);waitKey(0);
//				cout << "tamanio obj: " << tam_obj << " tamanio color: " << tam_color << endl;
//				imshow("H",imgSplit[0]);imshow("S",imgSplit[1]);imshow("V",imgSplit[2]);
//				imshow("salida",objeto[j]);
//				imshow("azul",img1);waitKey(0);
				
				double porcentaje = (tam_color/tam_obj)*100.0;
//				cout << "por " << porcentaje << endl;
				if (porcentaje>20.0) // si el 80 porciento del objeto es azul lo guardo
					contador++;
			}
			
			if (contador>1){ // 10 frames pasan la prueba
				bandera=true;
				cout << contador << endl;
			}
		}
		if (bandera)
			Objects_aux.push_back(Objects[i]);
	}
	cout << "asdsad: " << Objects.size() << " " << Objects_aux.size() << endl;
}
