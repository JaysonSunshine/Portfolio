/* Name: Jay Cloyd

   UID: 504209098

   Others With Whom I Discussed Things:

   Other Resources I Consulted:
   
*/

import java.io.*;
import java.util.stream.*;
import java.util.Arrays;
import java.util.*;
import java.util.concurrent.RecursiveTask;

// a marker for code that you need to implement
class ImplementMe extends RuntimeException {}

// an RGB triple
class RGB {
    public int R, G, B;

    RGB(int r, int g, int b) {
	R = r;
	G = g;
	B = b;
    }

    public RGB negate(int maxColorVal){
    	RGB negative = new RGB(maxColorVal - this.R, maxColorVal - this.G,
    		maxColorVal - this.B);
    	return negative;
    }

    public String toString() { return "(" + R + "," + G + "," + B + ")"; }

}

// code for creating a Gaussian filter
class Gaussian {

    protected static double gaussian(int x, int mu, double sigma) {
	return Math.exp( -(Math.pow((x-mu)/sigma,2.0))/2.0 );
    }

    public static double[][] gaussianFilter(int radius, double sigma) {
	int length = 2 * radius + 1;
	double[] hkernel = new double[length];
	for(int i=0; i < length; i++)
	    hkernel[i] = gaussian(i, radius, sigma);
	double[][] kernel2d = new double[length][length];
	double kernelsum = 0.0;
	for(int i=0; i < length; i++) {
	    for(int j=0; j < length; j++) {
		double elem = hkernel[i] * hkernel[j];
		kernelsum += elem;
		kernel2d[i][j] = elem;
	    }
	}
	for(int i=0; i < length; i++) {
	    for(int j=0; j < length; j++)
		kernel2d[i][j] /= kernelsum;
	}
	return kernel2d;
    }
}

class BlurHelper extends RecursiveTask<List<RGB>>{
	double[][] gfilter;
	protected int low, hi, width, height, radius;
	double sigma;
	protected RGB[] rgb_array;
	protected static final int SEQUENTIAL_CUTOFF = 8;

	public BlurHelper(RGB[] pixels, double[][] g, int w, int h, int r, double s, int l, int high){
		gfilter = g;
		rgb_array = pixels;
		low = l;
		hi = high;
		width = w;
		radius = r;
		sigma = s;
		height = h;
	}
	public List<RGB> compute(){
		if (hi - low < SEQUENTIAL_CUTOFF){
			List<RGB> l = new LinkedList<RGB>();
    		for(int i = low; i < hi; i++){
    			List<RGB> row_list = new LinkedList<RGB>();
    			for(int j = 0; j < width; j++){
    				long r = 0, g = 0, b = 0;
    				for(int rad_x = 0; rad_x < 2 * radius + 1; rad_x++){
    					for(int rad_y = 0; rad_y < 2 * radius + 1; rad_y++){
    						int h, w;
    						if(i - (radius - rad_x) < 0)
    							h = 0;
		                    else if(i - (radius - rad_x) > height - 1)
		                        h = height - 1;
		                    else
		                        h = i - (radius - rad_x);
		                    if(j - (radius - rad_y) < 0)
		                        w = 0;
		                    else if(j - (radius - rad_y) > width - 1)
		                        w = width - 1;
		                    else
		                        w = j - (radius - rad_y);
    						r += gfilter[rad_x][rad_y] * rgb_array[h * width + w].R;
    						g += gfilter[rad_x][rad_y] * rgb_array[h * width + w].G;
    						b += gfilter[rad_x][rad_y] * rgb_array[h * width + w].B;
    					}
    				}
    				row_list.add(new RGB(Math.round(r), Math.round(g), Math.round(b)));
    			}
    			l.addAll(row_list);
    		}
    		return l;
    	}
    	int mid = (low + hi) / 2;
    	BlurHelper t1 = new BlurHelper(rgb_array, gfilter, width, height, radius, sigma, low, mid);
    	BlurHelper t2 = new BlurHelper(rgb_array, gfilter, width, height, radius, sigma, mid, hi);
    	t1.fork();
    	List<RGB> l2 = t2.compute(); 
    	List<RGB> l1 = t1.join();
    	l1.addAll(l2);
    	return l1;
	}

}

class MirrorHelper extends RecursiveTask<List<RGB>>{
	protected int low, hi, width, height;
	protected RGB[] rgb_array;
	protected static final int SEQUENTIAL_CUTOFF = 100;

	public MirrorHelper(RGB[] pixels, int w, int l, int high){
		rgb_array = pixels;
		low = l;
		hi = high;
		width = w;	
	}
	public List<RGB> compute(){
		if (hi - low < SEQUENTIAL_CUTOFF){
			List<RGB> l = new LinkedList<RGB>();
    		for(int i = low; i < hi; i++){
    			RGB[] row = new RGB[width];
    			row = Arrays.copyOfRange(rgb_array, i * width, ((i + 1) * width));
    			List<RGB> list = Arrays.asList(row);
    			Collections.reverse(list);
    			l.addAll(list);
    		}
    		return l;
    	}
    	int mid = (low + hi) / 2;
    	MirrorHelper t1 = new MirrorHelper(rgb_array, width, low, mid);
    	MirrorHelper t2 = new MirrorHelper(rgb_array, width, mid, hi);
    	t1.fork();
    	List<RGB> l2 = t2.compute(); 
    	List<RGB> l1 = t1.join();
    	l1.addAll(l2);
    	return l1;
	}

}

// an object representing a single PPM image
class PPMImage {
    protected int width, height, maxColorVal;
    protected RGB[] pixels;

    PPMImage(int w, int h, int m, RGB[] p) {
	width = w;
	height = h;
	maxColorVal = m;
	pixels = p;
    }

	// parse a PPM file to produce a PPMImage
    public static PPMImage fromFile(String fname) throws FileNotFoundException, IOException {
	FileInputStream is = new FileInputStream(fname);
	BufferedReader br = new BufferedReader(new InputStreamReader(is));
	br.readLine(); // read the P6
	String[] dims = br.readLine().split(" "); // read width and height
	int width = Integer.parseInt(dims[0]);
	int height = Integer.parseInt(dims[1]);
	int max = Integer.parseInt(br.readLine()); // read max color value
	br.close();

	is = new FileInputStream(fname);
	    // skip the first three lines
	int newlines = 0;
	while (newlines < 3) {
	    int b = is.read();
	    if (b == 10)
		newlines++;
	}

	int MASK = 0xff;
	int numpixels = width * height;
	byte[] bytes = new byte[numpixels * 3];
        is.read(bytes);
	RGB[] pixels = new RGB[numpixels];
	for (int i = 0; i < numpixels; i++) {
	    int offset = i * 3;
	    pixels[i] = new RGB(bytes[offset] & MASK, bytes[offset+1] & MASK, bytes[offset+2] & MASK);
	}

	return new PPMImage(width, height, max, pixels);
    }

	// write a PPMImage object to a file
    public void toFile(String fname) throws IOException {
	FileOutputStream os = new FileOutputStream(fname);

	String header = "P6\n" + width + " " + height + "\n" + maxColorVal + "\n";
	os.write(header.getBytes());

	int numpixels = width * height;
	byte[] bytes = new byte[numpixels * 3];
	int i = 0;
	for (RGB rgb : pixels) {
	    bytes[i] = (byte) rgb.R;
	    bytes[i+1] = (byte) rgb.G;
	    bytes[i+2] = (byte) rgb.B;
	    i += 3;
	}
	os.write(bytes);
    }

	// implement using Java 8 Streams
    public PPMImage negate() {
    	RGB[] negative_pixels = 
    		Arrays.stream(pixels)
	    	.parallel()
	    	.map(s -> s.negate(this.maxColorVal))
	    	.toArray(RGB[]::new);
	    PPMImage negative = new PPMImage(this.width, this.height, this.maxColorVal, negative_pixels);
	    return negative;
   }
    
	// implement using Java's Fork/Join library
    public PPMImage mirrorImage() {
    	int size = height * width;
    	MirrorHelper t = new MirrorHelper(pixels, width, 0, height);
    	RGB[] row = new RGB[size];
    	List<RGB> l = t.compute();
    	row = l.toArray(new RGB[0]);
		PPMImage new_image = new PPMImage(width, height, maxColorVal, row);
		return new_image;
    }

	// implement using Java's Fork/Join library
    public PPMImage gaussianBlur(int radius, double sigma) {
		double[][] filter = Gaussian.gaussianFilter(radius, sigma);
		BlurHelper t = new BlurHelper(pixels, filter, width, height, radius, sigma, 0, height);
		RGB[] row_final = new RGB[height * width];
		List<RGB> l = t.compute();
		row_final = l.toArray(new RGB[0]);
		PPMImage new_image = new PPMImage(width, height, maxColorVal, row_final);
		return new_image;
    }

	// implement using Java 8 Streams
    public PPMImage gaussianBlur2(int radius, double sigma) {
    	double[][] filter = Gaussian.gaussianFilter(radius, sigma);
    	RGB[] blur_pixels = IntStream.range(0, pixels.length)
    	.parallel()
    	.mapToObj(i -> {
    		RGB pixel = pixels[i];
		    long r = 0, g = 0, b = 0;
			for(int rad_x = 0; rad_x < 2 * radius + 1; rad_x++){
				for(int rad_y = 0; rad_y < 2 * radius + 1; rad_y++){
					int h, w;
					if((i / width) - (radius - rad_x) < 0)
						h = 0;
                    else if((i / width) - (radius - rad_x) > height - 1)
                        h = height - 1;
                    else
                        h = (i / width) - (radius - rad_x);
                    if(i - ((i / width) * width) - (radius - rad_y) < 0)
                        w = 0;
                    else if(i - ((i / width) * width) - (radius - rad_y) > width - 1)
                        w = width - 1;
                    else
                        w = (i - ((i / width) * width)) - (radius - rad_y);
					r += filter[rad_x][rad_y] * pixels[h * width + w].R;
					g += filter[rad_x][rad_y] * pixels[h * width + w].G;
					b += filter[rad_x][rad_y] * pixels[h * width + w].B;
				}
			}
			return new RGB(Math.round(r), Math.round(g), Math.round(b));})
    	.toArray(RGB[]::new);
    	PPMImage blur = new PPMImage(width, height, maxColorVal, blur_pixels);
	    return blur;
    }
}

class Main{
	public static void main(String[] args){
		RGB[] pixels = new RGB[0];
		PPMImage image = new PPMImage(0, 0, 0, pixels);
		PPMImage image_negate = new PPMImage(0, 0, 0, pixels);
		PPMImage image_mirror = new PPMImage(0, 0, 0, pixels);
		PPMImage image_blur = new PPMImage(0, 0, 0, pixels);
		PPMImage image_blur2 = new PPMImage(0, 0, 0, pixels);
		try { 
			image = PPMImage.fromFile(args[0]);
		} catch(FileNotFoundException e){
			System.out.println("FileNotFoundException");
		} catch(IOException f){
			System.out.println("IOException");
		}
		image_negate = image.negate();
		image_mirror = image.mirrorImage();
		image_blur = image.gaussianBlur(2, 1.0);
		image_blur2 = image.gaussianBlur2(2,1.0);
		try {
			image.toFile(args[1]);
		} catch(IOException e){
			System.out.println("Couldn't write file");
		}
		try {
			image_negate.toFile(args[2]);
		} catch(IOException e){
			System.out.println("Couldn't write file");
		}
		try {
			image_mirror.toFile(args[3]);
		} catch(IOException e){
			System.out.println("Couldn't write file");
		}
		try {
			image_blur.toFile(args[4]);
		} catch(IOException e){
			System.out.println("Couldn't write file");
		}
		try {
			image_blur2.toFile(args[5]);
		} catch(IOException e){
			System.out.println("Couldn't write file");
		}
	}
}