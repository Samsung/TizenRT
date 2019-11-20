package smartfs_dump_parser.parts;

import javax.annotation.PostConstruct;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

public class GeometryViewer {

	private Label sectorSizeLabel;
	private Label sectorNumLabel;
	private Text sectorSizeText;
	private Text sectorNumText;

	public void updateGeometry(int sectorSize, int numberOfSectors) {
		sectorSizeText.setText(sectorSize + "");
		sectorSizeText.redraw();
		sectorSizeText.setEditable(false);
		
		sectorNumText.setText(numberOfSectors + "");
		sectorNumText.redraw();
		sectorNumText.setEditable(false);
	}
	
	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new GridLayout(2, true));

		sectorSizeLabel = new Label(parent, SWT.BORDER);
		sectorSizeLabel.setText("Sector Size: ");
		sectorSizeLabel.setSize(50, 20);
		sectorSizeText = new Text(parent, 0);
		sectorSizeText.setSize(50, 20);
		
		sectorNumLabel = new Label(parent, SWT.BORDER);
		sectorNumLabel.setText("Number of Sectors: ");
		sectorNumText = new Text(parent, 0);
		sectorNumText.setSize(80, 40);
	}
}
